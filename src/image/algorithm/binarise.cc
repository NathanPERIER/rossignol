#include "rossignol/image/algorithm/binarise.hh"

#include "image/algorithm/pixel_mapper.hh"


namespace {

uint8_t fs_correction(uint8_t grey, double correction) {
    const int16_t res = static_cast<int16_t>(grey) + static_cast<int16_t>(correction);
    if(res < 0) [[unlikely]] {
        return 0;
    }
    if(res > 255) [[unlikely]] {
        return 255;
    }
    return static_cast<uint8_t>(res);
}

bool fs_threshold(uint8_t grey) { return (grey > 127); }

int16_t fs_diff(uint8_t grey, bool bw) {
    return static_cast<int16_t>(grey) - (bw ? 255 : 0);
}

} // anonymous namespace


namespace rol::detail {

binary_image floyd_steinberg(const greyscale_image& img) {
    binary_image res(img.width(), img.height());

    /// @brief Correction values for the current row
    std::vector<double> correction(img.width(), 0.0);
    /// @brief Correction values for the next row
    std::vector<double> correction_next(img.width(), 0.0);

    for(std::size_t y = 0; y < img.height() - 1; y++) {
        std::span<const greyscalea> row = img[y];
        std::span<bool> out_row = res[y];
        uint8_t grey = ::fs_correction(row[0].grey, correction[0]);
        out_row[0] = ::fs_threshold(grey);
        int16_t diff = ::fs_diff(grey, out_row[0]);
        // TODO check width >= 1
        correction[1]     += static_cast<double>(8 * diff) / 16.0;
        correction_next[0] = static_cast<double>(6 * diff) / 16.0;
        correction_next[1] = static_cast<double>(2 * diff) / 16.0;
        for(std::size_t x = 1; x < img.width() - 1; x++) {
            grey = ::fs_correction(row[x].grey, correction[x]);
            out_row[x] = ::fs_threshold(grey);
            diff = ::fs_diff(grey, out_row[x]);
            correction[x+1]      += static_cast<double>(7 * diff) / 16.0;
            correction_next[x-1] += static_cast<double>(3 * diff) / 16.0;
            correction_next[x]   += static_cast<double>(5 * diff) / 16.0;
            correction_next[x+1]  = static_cast<double>(1 * diff) / 16.0;
        }
        grey = ::fs_correction(row[img.width()-1].grey, correction[img.width()-1]);
        out_row[img.width()-1] = ::fs_threshold(grey);
        diff = ::fs_diff(grey, out_row[img.width()-1]);
        correction_next[img.width()-2] += static_cast<double>(6 * diff) / 16.0;
        correction_next[img.width()-1] += static_cast<double>(10 * diff) / 16.0;
        std::swap(correction, correction_next);
    }
    std::span<const greyscalea> last_row = img[img.height()-1];
    std::span<bool> out_row = res[res.height()-1];
    for(std::size_t x = 1; x < img.width() - 1; x++) {
        uint8_t grey = ::fs_correction(last_row[x].grey, correction[x]);
        out_row[x] = ::fs_threshold(grey);
        correction[x+1] += static_cast<double>(::fs_diff(grey, out_row[x]));
    }
    uint8_t grey = ::fs_correction(last_row[img.width()-1].grey, correction[img.width()-1]);
    out_row[img.width()-1] = ::fs_threshold(grey);

    return res;
}

binary_image binarise_threshold(const greyscale_image& img, uint8_t threshold) {
    return map_pixels<bool>(img, [&threshold](const greyscalea& pixel) {
        return (pixel.grey > threshold);
    });
}

} // namespace rol::detail


namespace {

class binarise_greyscale_impl {
public:
    binarise_greyscale_impl(const rol::greyscale_image& img): _img(img) {}

    rol::binary_image operator()(rol::floyd_steinberg_t) {
        return rol::detail::floyd_steinberg(_img);
    }
    rol::binary_image operator()(const rol::binary_threshold& threshold) {
        return rol::detail::binarise_threshold(_img, threshold.value);
    }

private:
    const rol::greyscale_image& _img;
};

class binarise_general_impl {
public:
    binarise_general_impl(const rol::binarise_method& method): _method(method) {}

    rol::binary_image operator()(const rol::binary_image& img) {
        return img.share();
    }
    rol::binary_image operator()(const rol::rgb_image&) {
        throw std::runtime_error("Unable to binarise an RGB image, convert it to greyscale first");
    }
    rol::binary_image operator()(const rol::layer&) {
        throw std::runtime_error("Unable to binarise an unnamed layer, convert it to greyscale first");
    }
    rol::binary_image operator()(const rol::coefficient_plane&) {
        throw std::runtime_error("Unable to binarise a coefficient plane (maybe convert to greyscale ?)");
    }
    rol::binary_image operator()(const rol::greyscale_image& img) {
        return std::visit(binarise_greyscale_impl(img), _method);
    }

private:
    const rol::binarise_method& _method;
};

} // anonymous namespace


namespace rol {

binary_image binarise(const image& img, binarise_method method) {
    return std::visit(::binarise_general_impl(method), img);
}

} // namespace rol
