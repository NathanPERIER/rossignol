#include "rossignol/image/algorithm/greyscale.hh"

#include "image/algorithm/pixel_mapper.hh"


namespace rol::detail {

template <luma_method Method>
greyscale_image greyscale(const rgb_image& img) {
    return map_pixels<greyscalea>(img, [](const rgba& pixel) {
        return greyscalea {
            .grey = luma<Method>(pixel),
            .a = pixel.a
        };
    });
}

greyscale_image greyscale(const binary_image& img) {
    return map_pixels<greyscalea>(img, [](bool pixel) {
        return greyscalea {
            .grey = static_cast<uint8_t>(pixel ? 255 : 0),
            .a = 255
        };
    });
}

greyscale_image greyscale(const layer& img) {
    return map_pixels<greyscalea>(img, [](uint8_t pixel) {
        return greyscalea {
            .grey = pixel,
            .a = 255
        };
    });
}

greyscale_image greyscale(const coefficient_plane& img) {
    return map_pixels<greyscalea>(img, [](double pixel) {
        return greyscalea {
            .grey = static_cast<uint8_t>(pixel * 255.0),
            .a = 255
        };
    });
}

} // namespace rol::detail


namespace {

class greyscale_impl {
public:
    greyscale_impl(rol::luma_method method): _method(method) {}

    rol::greyscale_image operator()(const rol::greyscale_image& img) {
        return img.share();
    }
    rol::greyscale_image operator()(const rol::binary_image& img) {
        return rol::detail::greyscale(img);
    }
    rol::greyscale_image operator()(const rol::layer& img) {
        return rol::detail::greyscale(img);
    }
    rol::greyscale_image operator()(const rol::coefficient_plane& img) {
        return rol::detail::greyscale(img);
    }
    rol::greyscale_image operator()(const rol::rgb_image& img) {
        switch(_method) {
            case rol::luma_method::sdtv: return rol::detail::greyscale<rol::luma_method::sdtv>(img);
            case rol::luma_method::hdtv: return rol::detail::greyscale<rol::luma_method::hdtv>(img);
            case rol::luma_method::hdr:  return rol::detail::greyscale<rol::luma_method::hdr>(img);
            default: throw std::runtime_error("Unsupported luma method");
        }
    }

private:
    rol::luma_method _method;
};

} // anonymous namespace


namespace rol {

greyscale_image to_greyscale(const image& img, luma_method method) {
    return std::visit(greyscale_impl(method), img);
}

} // namespace rol