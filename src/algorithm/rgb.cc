#include "rossignol/algorithm/rgb.hh"

#include "rossignol/algorithm/map_pixels.hh"
#include "rossignol/colour/conv/greyscale.hh"
#include "rossignol/colour/conv/numeric.hh"


namespace rol::algo {

rgb_image to_rgb(const greyscale_image& img) {
    return map_pixels<rgba>(img, [](const greyscalea& pixel) {
        return colour_conv<rgba>(pixel);
    });
}

rgb_image to_rgb(const binary_image& img) {
    return map_pixels<rgba>(img, [](const bool& pixel) {
        return colour_conv<rgba>(pixel);
    });
}

rgb_image to_rgb(const layer& img) {
    return map_pixels<rgba>(img, [](const uint8_t& pixel) {
        return colour_conv<rgba>(pixel);
    });
}

rgb_image to_rgb(const coefficient_plane& img) {
    return map_pixels<rgba>(img, [](const double& pixel) {
        return colour_conv<rgba>(pixel);
    });
}

} // namespace rol::algo
