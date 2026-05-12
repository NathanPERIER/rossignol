#include "rossignol/algorithm/rgb.hh"

#include "rossignol/algorithm/map_pixels.hh"
#include "rossignol/colour/conv/greyscale.hh"


namespace rol::algo {

rgb_image to_rgb(const greyscale_image& img) {
    return map_pixels<rgba>(img, colour_conv<rgba>);
}

rgb_image to_rgb(const binary_image& img) {
    return map_pixels<rgba>(img, [](const bool& pixel) {
        const uint8_t value = static_cast<uint8_t>(pixel ? 255 : 0);
        return rgba {
            .r = value,
            .g = value,
            .b = value,
            .a = 255
        };
    });
}

rgb_image to_rgb(const layer& img) {
    return map_pixels<rgba>(img, [](const uint8_t& pixel) {
        return rgba {
            .r = pixel,
            .g = pixel,
            .b = pixel,
            .a = 255
        };
    });
}

rgb_image to_rgb(const coefficient_plane& img) {
    return map_pixels<rgba>(img, [](const double& pixel) {
        const uint8_t value = static_cast<uint8_t>(pixel * 255.0);
        return rgba {
            .r = value,
            .g = value,
            .b = value,
            .a = 255
        };
    });
}

} // namespace rol::algo
