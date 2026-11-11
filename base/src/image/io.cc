#include "rossignol/image/io.hh"

#include <spdlog/spdlog.h>

#include <png.h>

#include "rossignol/utils/errors/assert.hh"
#include "rossignol/utils/io/io_error.hh"


namespace {

void error_callback(png_struct*, png_const_charp msg) {
    throw rol::io_error(fmt::format("Got PNG error: {}", msg));
}

void warn_callback(png_struct*, png_const_charp msg) {
    SPDLOG_WARN("PNG warning: {}", msg);
}


class read_structs_holder {
public:
    png_struct* png        = nullptr;
    png_info*   begin_info = nullptr;
    png_info*   end_info   = nullptr;

    read_structs_holder(read_structs_holder&&) = default;
    read_structs_holder& operator=(read_structs_holder&&) = default;

    read_structs_holder(const read_structs_holder&) = delete;
    read_structs_holder& operator=(const read_structs_holder&) = delete;

    static read_structs_holder make() {
        read_structs_holder res;

        res.png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, error_callback, warn_callback);
        if(res.png == nullptr) {
            throw rol::io_error("Failed to create PNG read structure");
        }

        res.begin_info = png_create_info_struct(res.png);
        if(res.begin_info == nullptr) {
            throw rol::io_error("Failed to create PNG begin info structure");
        }

        res.end_info = png_create_info_struct(res.png);
        if(res.end_info == nullptr) {
            throw rol::io_error("Failed to create PNG begin info structure");
        }

        return res;
    }

    ~read_structs_holder() {
        if(png == nullptr && begin_info == nullptr && end_info == nullptr) {
            return;
        }
        png_destroy_read_struct(&png, &begin_info, &end_info);
        png        = nullptr;
        begin_info = nullptr;
        end_info   = nullptr;
    }

private:
    read_structs_holder() = default;
};


class write_structs_holder {
public:
    png_struct* png  = nullptr;
    png_info*   info = nullptr;

    write_structs_holder(write_structs_holder&&) = default;
    write_structs_holder& operator=(write_structs_holder&&) = default;

    write_structs_holder(const write_structs_holder&) = delete;
    write_structs_holder& operator=(const write_structs_holder&) = delete;

    static write_structs_holder make() {
        write_structs_holder res;

        res.png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, error_callback, warn_callback);
        if(res.png == nullptr) {
            throw rol::io_error("Failed to create PNG write structure");
        }

        res.info = png_create_info_struct(res.png);
        if(res.info == nullptr) {
            throw rol::io_error("Failed to create PNG info structure");
        }

        return res;
    }

    ~write_structs_holder() {
        if(png == nullptr && info == nullptr) {
            return;
        }
        png_destroy_write_struct(&png, &info);
    }

private:
    write_structs_holder() = default;
};


void read_data_callback(png_struct* png, png_byte* data, png_size_t length) {
    std::span<uint8_t>* buf = reinterpret_cast<std::span<uint8_t>*>(png_get_io_ptr(png));
    if(length > buf->size()) {
        png_error(png, "Could not load enough data from PNG buffer");
    }
    memcpy(data, buf->data(), length);
    *buf = buf->subspan(length);
}

void write_data_callback(png_struct* png, png_byte* data, png_size_t size) {
    std::vector<uint8_t>* buf = reinterpret_cast<std::vector<uint8_t>*>(png_get_io_ptr(png));
    buf->insert(buf->end(), data, data + size);
}

void flush_data_callback(png_struct*) { /* nothing to do here */ }


/// @brief helper to read a colour without alpha to a struct with alpha
template <typename Colour>
Colour add_alpha(std::span<uint8_t> pixel_data) {
    if constexpr (std::same_as<Colour, rol::rgba>) {
        return rol::rgba {
            .r = pixel_data[0],
            .g = pixel_data[1],
            .b = pixel_data[2],
            .a = 255,
        };
    } else {
        static_assert(std::same_as<Colour, rol::greyscalea>);
        return rol::greyscalea {
            .grey = pixel_data[0],
            .a = 255
        };
    }
}

/// @brief helper to read pixels from an image (RGB or greyscale) that doesn't have alpha
/// @note unfortunately this requires a copy
template <typename Colour>
requires(std::same_as<Colour, rol::rgba> || std::same_as<Colour, rol::greyscalea>)
rol::basic_image<Colour> read_pixels_no_alpha(read_structs_holder& holder, const std::size_t row_bytes, const std::size_t height) {
    static_assert(sizeof(Colour) > 1);
    static constexpr std::size_t source_pixel_size = sizeof(Colour) - 1;
    ROL_ASSERT(row_bytes % source_pixel_size == 0, "The row size should be a multiple of the pixel size (in bytes)");
    const std::size_t width = row_bytes / source_pixel_size;
    std::vector<uint8_t> pixels(height * row_bytes);
    std::vector<uint8_t*> rows;
    rows.reserve(height);
    for(std::size_t y = 0; y < height; y++) {
        rows.push_back(pixels.data() + row_bytes * y);
    }
    png_read_image(holder.png, rows.data());
    rol::basic_image<Colour> res(rol::math::vec2u { .x = width, .y = height});
    for(std::size_t y = 0; y < height; y++) {
        std::span<uint8_t> source_row(rows[y], row_bytes);
        std::span<Colour> alpha_row = res[y];
        for(std::size_t x = 0; x < width; x++) {
            alpha_row[x] = add_alpha<Colour>(source_row.subspan(x * source_pixel_size, source_pixel_size));
        }
    }
    return res;
}

/// @brief helper to read pixels from an image (RGB or greyscale) with alpha
template <typename Colour>
requires(std::same_as<Colour, rol::rgba> || std::same_as<Colour, rol::greyscalea>)
rol::basic_image<Colour> read_pixels_with_alpha(read_structs_holder& holder, const std::size_t row_bytes, const std::size_t height) {
    static constexpr std::size_t pixel_size = sizeof(Colour);
    ROL_ASSERT(row_bytes % pixel_size == 0, "The row size should be a multiple of the pixel size (in bytes)");
    const std::size_t width = row_bytes / pixel_size;
    // The structs are already in the correct order wo se can read directly to the buffer with a cast
    rol::basic_image<Colour> res(rol::math::vec2u { .x = width, .y = height });
    std::vector<uint8_t*> rows;
    rows.reserve(height);
    for(std::size_t y = 0; y < height; y++) {
        rows.push_back(reinterpret_cast<uint8_t*>(res[y].data()));
    }
    png_read_image(holder.png, rows.data());
    return res;
}

// TODO logging
rol::parsed_image_t read_pixels(read_structs_holder& holder, const std::size_t row_bytes, const std::size_t height) {
    int bit_depth = 0;
    int colour_type = 0;
    int interlace_type = 0;

    if(png_get_IHDR(holder.png, holder.begin_info, nullptr, nullptr, &bit_depth, &colour_type, &interlace_type, nullptr, nullptr) == 0) [[unlikely]] {
        throw rol::io_error("Unable to read IHDR metadata from PNG image");
    }

    if(bit_depth != 8) {
        throw rol::io_error(fmt::format("Only 8-bit colour channel depth is supported (got bit_depth={})", bit_depth));
    }

    if(interlace_type != PNG_INTERLACE_NONE) [[unlikely]] {
        throw rol::io_error("Interlacing is not supported");
    }

    switch(colour_type) {
        case PNG_COLOR_TYPE_GRAY:       return read_pixels_no_alpha<rol::greyscalea>(holder, row_bytes, height);
        case PNG_COLOR_TYPE_GRAY_ALPHA: return read_pixels_with_alpha<rol::greyscalea>(holder, row_bytes, height);
        case PNG_COLOR_TYPE_RGB:        return read_pixels_no_alpha<rol::rgba>(holder, row_bytes, height);
        case PNG_COLOR_TYPE_RGB_ALPHA:  return read_pixels_with_alpha<rol::rgba>(holder, row_bytes, height);
        case PNG_COLOR_TYPE_PALETTE:    throw rol::io_error("Palette images are currently not supported");
        default: throw rol::io_error("Unknown colour type in PNG image");
    }
}


void list_chunks(png_struct* png, png_info* info) {
    constexpr std::size_t nb_flags = 16;
    constexpr std::array<png_uint_32, nb_flags> flags {
        PNG_INFO_gAMA, PNG_INFO_sBIT, PNG_INFO_cHRM, PNG_INFO_PLTE, PNG_INFO_tRNS,
        PNG_INFO_bKGD, PNG_INFO_hIST, PNG_INFO_pHYs, PNG_INFO_oFFs, PNG_INFO_tIME,
        PNG_INFO_pCAL, PNG_INFO_sRGB, PNG_INFO_iCCP, PNG_INFO_sPLT, PNG_INFO_sCAL,
        PNG_INFO_IDAT
    };
    constexpr std::array<std::string_view, nb_flags> flag_names {
        "gAMA", "sBIT", "cHRM", "PLTE", "tRNS", "bKGD", "hIST", "pHYs",
        "oFFs", "tIME", "pCAL", "sRGB", "iCCP", "sPLT", "sCAL", "IDAT"
    };

    std::string log_line("  - flags:");
    std::back_insert_iterator format_it = std::back_inserter(log_line);

    bool flag_found = false;
    for(std::size_t i = 0; i < nb_flags; i++) {
        if(png_get_valid(png, info, flags[i] ) != 0) {
            fmt::format_to(format_it, " {}", flag_names[i]);
            flag_found = true;
        }
    }

    if(!flag_found) {
        fmt::format_to(format_it, " (none)");
    }

    SPDLOG_TRACE("{}", log_line);
    log_line.clear();

    fmt::format_to(format_it, "  - texts:");
    png_text* texts = nullptr;
    std::size_t nb_texts = png_get_text(png, info, &texts, nullptr);
    if(nb_texts > 0) {
        std::size_t tN = 0, zN = 0, iN = 0, izN = 0, unk = 0;
        for(std::size_t i = 0; i < nb_texts; i++) {
            switch(texts[i].compression) {
                case PNG_TEXT_COMPRESSION_NONE: tN++;  break;
                case PNG_TEXT_COMPRESSION_zTXt: zN++;  break;
                case PNG_ITXT_COMPRESSION_NONE: iN++;  break;
                case PNG_ITXT_COMPRESSION_zTXt: izN++; break;
                default: unk++;
            }
        }

        // collect names and number of chunks
        if(tN > 0)  { fmt::format_to(format_it, " tEXt({})", tN); }
        if(zN > 0)  { fmt::format_to(format_it, " zTXt({})", zN); }
        if(iN > 0)  { fmt::format_to(format_it, " iTXt({})", iN); }
        if(izN > 0) { fmt::format_to(format_it, " iTXt-compressed({})", izN); }
        if(unk > 0) { fmt::format_to(format_it, " unknown({})", unk); }
    } else {
        fmt::format_to(format_it, " (none)");
    }

    SPDLOG_TRACE("{}", log_line);
    log_line.clear();

    png_unknown_chunk* unknowns;
    std::size_t nb_unknowns = png_get_unknown_chunks(png, info, &unknowns);
    if(nb_unknowns > 0) {
        SPDLOG_TRACE("  - unknowns:");
        for(std::size_t i = 0; i < nb_unknowns; i++) {
            SPDLOG_TRACE("    - {} ({}B)\n", reinterpret_cast<const char*>(unknowns[i].name), unknowns[i].size);
        }
    } else {
        SPDLOG_TRACE("  - unknowns: (none)");
    }
}

void print_info(read_structs_holder& holder) {
    png_uint_32 width = 0;
    png_uint_32 height = 0;
    int bit_depth = 0;
    int colour_type = 0;
    int interlace_type = 0;

    if(png_get_IHDR(holder.png, holder.begin_info, &width, &height, &bit_depth, &colour_type, &interlace_type, nullptr, nullptr) == 0) {
        return;
    }

    std::string log_line;
    std::back_insert_iterator format_it = std::back_inserter(log_line);

    fmt::format_to(format_it, "{}x{} {}-bit", width, height, bit_depth);
    switch(colour_type) {
        case PNG_COLOR_TYPE_GRAY:       fmt::format_to(format_it, " grayscale");       break;
        case PNG_COLOR_TYPE_GRAY_ALPHA: fmt::format_to(format_it, " gray with alpha"); break;
        case PNG_COLOR_TYPE_PALETTE:    fmt::format_to(format_it, " palette");         break;
        case PNG_COLOR_TYPE_RGB:        fmt::format_to(format_it, " RGB");             break;
        case PNG_COLOR_TYPE_RGB_ALPHA:  fmt::format_to(format_it, " RGBA");            break;
        default: fmt::format_to(format_it, " Unknown colour type");
    }

    if(interlace_type == PNG_INTERLACE_ADAM7) {
        fmt::format_to(format_it, " interlaced");
    }

    SPDLOG_TRACE("{}", log_line);
    log_line.clear();

    SPDLOG_TRACE("Chunks before IDAT:");
    list_chunks(holder.png, holder.begin_info);
    SPDLOG_TRACE("Chunks after IDAT:");
    list_chunks(holder.png, holder.end_info);
}


template <int PngColourType, typename Colour>
std::vector<uint8_t> dump_image_impl(rol::basic_image<Colour>& img) {
    ::write_structs_holder holder = ::write_structs_holder::make();

    png_set_IHDR(
        holder.png,
        holder.info,
        img.width(),
        img.height(),
        8,
        PngColourType,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );

    // const size_t row_bytes = png_get_rowbytes(holder.png, holder.info);
    // assert(row_bytes == (_width * sizeof(npp::rgba)));

    std::vector<png_byte*> row_pointers;
    row_pointers.reserve(img.height());
    for(size_t y = 0; y < img.height(); y++) {
        row_pointers.push_back(reinterpret_cast<png_byte*>(img[y].data()));
    }

    std::vector<uint8_t> res;
    png_set_write_fn(holder.png, &res, write_data_callback, flush_data_callback);

    png_write_info(holder.png, holder.info);
    png_write_image(holder.png, row_pointers.data()); // Warning: systematic copy here
    png_write_end(holder.png, nullptr);

    return res;
}

} // anonymous namespace


namespace rol {

parsed_image_t parse_image(std::span<const uint8_t> raw) {
    static constexpr std::size_t sig_size = 8;
    if(raw.size() < sig_size || png_sig_cmp(raw.data(), 0, sig_size) != 0) {
        throw rol::io_error("Bad PNG signature");
    }

    ::read_structs_holder holder = ::read_structs_holder::make();

    std::span<const uint8_t> read_buf = raw.subspan(sig_size);
    png_set_read_fn(holder.png, reinterpret_cast<void*>(&read_buf), read_data_callback);
    png_set_sig_bytes(holder.png, sig_size);
    png_set_keep_unknown_chunks(holder.png, PNG_HANDLE_CHUNK_ALWAYS, nullptr, 0);

    png_read_info(holder.png, holder.begin_info);

    const std::size_t row_bytes = png_get_rowbytes(holder.png, holder.begin_info);
    const std::size_t height = png_get_image_height(holder.png, holder.begin_info);
    parsed_image_t res = read_pixels(holder, row_bytes, height);

    png_read_end(holder.png, holder.end_info);

    if(spdlog::get_level() <= spdlog::level::trace) {
        ::print_info(holder);
    }

    return res;
}


std::vector<uint8_t> dump_image(rol::rgb_image& img) {
    return ::dump_image_impl<PNG_COLOR_TYPE_RGBA>(img);
}

std::vector<uint8_t> dump_image(rol::greyscale_image& img) {
    return ::dump_image_impl<PNG_COLOR_TYPE_GRAY_ALPHA>(img);
}

} // namespace rol
