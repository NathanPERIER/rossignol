
#pragma once

#include "rossignol/algorithm/map_pixels.hh"
#include "rossignol/colour/conv/cylindrical.hh"
#include "rossignol/image/image.hh"
#include "rossignol/params/colourise.hh"
#include "rossignol/utils/typing/any_of.hh"


namespace rol::algo::detail {

template <typename CylindricalColour>
requires(any_of<CylindricalColour, hsl, hsv, hsi>)
CylindricalColour cylindrical_from_greyscale(const double& hue, const double& saturation, const double& grey_coeff);

template <>
hsl cylindrical_from_greyscale(const double& hue, const double& saturation, const double& grey_coeff) {
    return hsl { .h = hue, .s = saturation, .l = grey_coeff };
}

template <>
hsv cylindrical_from_greyscale(const double& hue, const double& saturation, const double& grey_coeff) {
    return hsv { .h = hue, .s = saturation, .v = grey_coeff };
}

template <>
hsi cylindrical_from_greyscale(const double& hue, const double& saturation, const double& grey_coeff) {
    return hsi { .h = hue, .s = saturation, .i = grey_coeff };
}

template <typename CylindricalColour>
requires(any_of<CylindricalColour, hsl, hsv, hsi>)
rgba rgba_from_greyscale_via_cylindrical(const double& hue, const double& saturation, const double& grey_coeff) {
    return colour_conv<rgba>(cylindrical_from_greyscale<CylindricalColour>(hue, saturation, grey_coeff));
}

} // namespace rol::algo::detail


namespace rol::algo {

rgb_image colourise_binary(const binary_image& img, const rgba& col_one, const rgba& col_zero) {
    return map_pixels<rgba>(img, [&col_one, &col_zero](const bool& pixel) -> rgba {
        return pixel ? col_one : col_zero;
    });
}

template <typename CylindricalColour>
requires(any_of<CylindricalColour, hsl, hsv, hsi>)
rgb_image colourise(const coefficient_plane& img, double hue = defaults::colourise_hue, double saturation = defaults::colourise_saturation) {
    return map_pixels<rgba>(img, [&hue, &saturation](const double& pixel) -> rgba {
        return detail::rgba_from_greyscale_via_cylindrical<CylindricalColour>(hue, saturation, pixel);
    });
}

template <typename CylindricalColour>
requires(any_of<CylindricalColour, hsl, hsv, hsi>)
rgb_image colourise(const layer& img, double hue = defaults::colourise_hue, double saturation = defaults::colourise_saturation) {
    return map_pixels<rgba>(img, [&hue, &saturation](const uint8_t& pixel) -> rgba {
        return detail::rgba_from_greyscale_via_cylindrical<CylindricalColour>(hue, saturation, static_cast<double>(pixel) / 255.0);
    });
}

template <typename CylindricalColour>
requires(any_of<CylindricalColour, hsl, hsv, hsi>)
rgb_image colourise(const greyscale_image& img, double hue = defaults::colourise_hue, double saturation = defaults::colourise_saturation) {
    return map_pixels<rgba>(img, [&hue, &saturation](const greyscalea& pixel) -> rgba {
        rgba res = detail::rgba_from_greyscale_via_cylindrical<CylindricalColour>(hue, saturation, static_cast<double>(pixel.grey) / 255.0);
        res.a = pixel.a;
        return res;
    });
}

} // namespace rol::algo
