
#pragma once

#include "rossignol/algorithm/map_pixels.hh"
#include "rossignol/colour/conv/numeric.hh"
#include "rossignol/colour/conv/cylindrical.hh"
#include "rossignol/image/image.hh"
#include "rossignol/params/coefficient_type.hh"


namespace rol::algo::detail {

template <typename Colour, coefficient_type Coeff>
struct coefficient_extractor;

template <>
struct coefficient_extractor<rgba, coefficient_type::red> {
    double operator()(const rgba& col) const { return colour_conv<double>(col.r); }
};

template <>
struct coefficient_extractor<rgba, coefficient_type::green> {
    double operator()(const rgba& col) const { return colour_conv<double>(col.g); }
};

template <>
struct coefficient_extractor<rgba, coefficient_type::blue> {
    double operator()(const rgba& col) const { return colour_conv<double>(col.b); }
};

template <>
struct coefficient_extractor<rgba, coefficient_type::alpha> {
    double operator()(const rgba& col) const { return colour_conv<double>(col.a); }
};

template <>
struct coefficient_extractor<rgba, coefficient_type::hue> {
    double operator()(const rgba& col) const { return hue_coeff(col); }
};

template <>
struct coefficient_extractor<rgba, coefficient_type::chroma> {
    double operator()(const rgba& col) const { return chroma(col); }
};

template <>
struct coefficient_extractor<rgba, coefficient_type::hsi_saturation> {
    double operator()(const rgba& col) const { return saturation<hsi>(col); }
};

template <>
struct coefficient_extractor<rgba, coefficient_type::hsl_saturation> {
    double operator()(const rgba& col) const { return saturation<hsl>(col); }
};

template <>
struct coefficient_extractor<rgba, coefficient_type::hsv_saturation> {
    double operator()(const rgba& col) const { return saturation<hsv>(col); }
};

template <>
struct coefficient_extractor<rgba, coefficient_type::lightness> {
    double operator()(const rgba& col) const { return lightness_coeff(col); }
};

template <>
struct coefficient_extractor<rgba, coefficient_type::value> {
    double operator()(const rgba& col) const { return value_coeff(col); }
};

template <>
struct coefficient_extractor<rgba, coefficient_type::intensity> {
    double operator()(const rgba& col) const { return intensity_coeff(col); }
};

template <>
struct coefficient_extractor<greyscalea, coefficient_type::grey> {
    double operator()(const greyscalea& col) const { return colour_conv<double>(col.grey); }
};

template <>
struct coefficient_extractor<greyscalea, coefficient_type::alpha> {
    double operator()(const greyscalea& col) const { return colour_conv<double>(col.a); }
};

template <coefficient_type Coeff, typename Colour>
concept extractible_coefficient = yield_invocable<const coefficient_extractor<Colour, Coeff>&, double, const Colour&>;

} // namespace rol::algo::detail


namespace rol::algo {

coefficient_plane to_coefficients(const layer& img);

/*--------------------+
|  RGBA               |
+--------------------*/

template <coefficient_type Coeff>
requires(detail::extractible_coefficient<Coeff, rol::rgba>)
inline coefficient_plane extract_coefficient(const rgb_image& img) {
    return map_pixels<double>(img, detail::coefficient_extractor<rol::rgba, Coeff> {});
}


/*--------------------+
|  GreyscaleA         |
+--------------------*/

template <coefficient_type Coeff>
requires(detail::extractible_coefficient<Coeff, rol::greyscalea>)
inline coefficient_plane extract_coefficient(const greyscale_image& img) {
    return map_pixels<double>(img, detail::coefficient_extractor<rol::greyscalea, Coeff> {});
}

} // namespace rol::algo
