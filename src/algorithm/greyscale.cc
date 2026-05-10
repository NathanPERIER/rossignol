#include "rossignol/algorithm/greyscale.hh"

#include "rossignol/colour/conv.hh"

#include "image/algorithm/pixel_mapper.hh"


namespace rol::algo {

template <luma_method Method>
greyscale_image greyscale_luma(const rgb_image& img) {
    return rol::detail::map_pixels<greyscalea>(img, [](const rgba& pixel) {
        return greyscalea {
            .grey = luma<Method>(pixel),
            .a = pixel.a
        };
    });
}

template greyscale_image greyscale_luma<luma_method::sdtv>(const rgb_image& img);
template greyscale_image greyscale_luma<luma_method::hdtv>(const rgb_image& img);
template greyscale_image greyscale_luma<luma_method::hdr>(const rgb_image& img);


greyscale_image greyscale_intensity(const rgb_image& img) {
    return rol::detail::map_pixels<greyscalea>(img, [](const rgba& pixel) {
        return greyscalea {
            .grey = intensity(pixel),
            .a = pixel.a
        };
    });
}

greyscale_image greyscale_lightness(const rgb_image& img) {
    return rol::detail::map_pixels<greyscalea>(img, [](const rgba& pixel) {
        return greyscalea {
            .grey = lightness(pixel),
            .a = pixel.a
        };
    });
}

greyscale_image greyscale_value(const rgb_image& img) {
    return rol::detail::map_pixels<greyscalea>(img, [](const rgba& pixel) {
        return greyscalea {
            .grey = value(pixel),
            .a = pixel.a
        };
    });
}


greyscale_image greyscale(const binary_image& img) {
    return rol::detail::map_pixels<greyscalea>(img, [](bool pixel) {
        return greyscalea {
            .grey = static_cast<uint8_t>(pixel ? 255 : 0),
            .a = 255
        };
    });
}

greyscale_image greyscale(const layer& img) {
    return rol::detail::map_pixels<greyscalea>(img, [](uint8_t pixel) {
        return greyscalea {
            .grey = pixel,
            .a = 255
        };
    });
}

greyscale_image greyscale(const coefficient_plane& img) {
    return rol::detail::map_pixels<greyscalea>(img, [](double pixel) {
        return greyscalea {
            .grey = static_cast<uint8_t>(pixel * 255.0),
            .a = 255
        };
    });
}

} // namespace rol::algo
