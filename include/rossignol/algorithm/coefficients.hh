
#pragma once

#include "rossignol/algorithm/map_pixels.hh"
#include "rossignol/algorithm/overlaying.hh"
#include "rossignol/colour/conv/numeric.hh"
#include "rossignol/colour/conv/cylindrical.hh"
#include "rossignol/image/image.hh"
#include "rossignol/params/coefficient_type.hh"


namespace rol::algo::detail {

// TODO we need a concept for colour conversion
template <typename IntermediaryColour, typename Colour, typename EditCallback>
requires(yield_invocable<EditCallback, void, IntermediaryColour&>)
void with_colour_space(Colour& col, const EditCallback& edit) {
    IntermediaryColour interm_col = colour_conv<IntermediaryColour>(col);
    edit(interm_col);
    col = colour_conv<Colour>(interm_col);
}

template <typename Colour, coefficient_type Coeff>
struct coefficient_extractor;

template <>
struct coefficient_extractor<rgba, coefficient_type::red> {
    double operator()(const rgba& col) const { return colour_conv<double>(col.r); }
    void operator()(rgba& col, const double& value) const { col.r = colour_conv<uint8_t>(value); }
};

template <>
struct coefficient_extractor<rgba, coefficient_type::green> {
    double operator()(const rgba& col) const { return colour_conv<double>(col.g); }
    void operator()(rgba& col, const double& value) const { col.g = colour_conv<uint8_t>(value); }
};

template <>
struct coefficient_extractor<rgba, coefficient_type::blue> {
    double operator()(const rgba& col) const { return colour_conv<double>(col.b); }
    void operator()(rgba& col, const double& value) const { col.b = colour_conv<uint8_t>(value); }
};

template <>
struct coefficient_extractor<rgba, coefficient_type::alpha> {
    double operator()(const rgba& col) const { return colour_conv<double>(col.a); }
    void operator()(rgba& col, const double& value) const { col.a = colour_conv<uint8_t>(value); }
};

template <>
struct coefficient_extractor<rgba, coefficient_type::hue> {
    double operator()(const rgba& col) const { return hue_coeff(col); }
    void operator()(rgba& col, const double& value) const {
        with_colour_space<hsl>(col, [&value](hsl& col) { col.h = value; });
    }
};

template <>
struct coefficient_extractor<rgba, coefficient_type::chroma> {
    double operator()(const rgba& col) const { return chroma(col); }
    // There is no easy way to set the chroma with the currently implemented colour spaces
    // Maybe this could be done via CIELch/Oklch ?
};

template <>
struct coefficient_extractor<rgba, coefficient_type::hsi_saturation> {
    double operator()(const rgba& col) const { return saturation<hsi>(col); }
    void operator()(rgba& col, const double& value) const {
        with_colour_space<hsi>(col, [&value](hsi& col) { col.s = value; });
    }
};

template <>
struct coefficient_extractor<rgba, coefficient_type::hsl_saturation> {
    double operator()(const rgba& col) const { return saturation<hsl>(col); }
    void operator()(rgba& col, const double& value) const {
        with_colour_space<hsl>(col, [&value](hsl& col) { col.s = value; });
    }
};

template <>
struct coefficient_extractor<rgba, coefficient_type::hsv_saturation> {
    double operator()(const rgba& col) const { return saturation<hsv>(col); }
    void operator()(rgba& col, const double& value) const {
        with_colour_space<hsv>(col, [&value](hsv& col) { col.s = value; });
    }
};

template <>
struct coefficient_extractor<rgba, coefficient_type::lightness> {
    double operator()(const rgba& col) const { return lightness_coeff(col); }
    void operator()(rgba& col, const double& value) const {
        with_colour_space<hsl>(col, [&value](hsl& col) { col.l = value; });
    }
};

template <>
struct coefficient_extractor<rgba, coefficient_type::value> {
    double operator()(const rgba& col) const { return value_coeff(col); }
    void operator()(rgba& col, const double& value) const {
        with_colour_space<hsv>(col, [&value](hsv& col) { col.v = value; });
    }
};

template <>
struct coefficient_extractor<rgba, coefficient_type::intensity> {
    double operator()(const rgba& col) const { return intensity_coeff(col); }
    void operator()(rgba& col, const double& value) const {
        with_colour_space<hsi>(col, [&value](hsi& col) { col.i = value; });
    }
};

template <>
struct coefficient_extractor<greyscalea, coefficient_type::grey> {
    double operator()(const greyscalea& col) const { return colour_conv<double>(col.grey); }
    void operator()(greyscalea& col, const double& value) const { col.grey = colour_conv<uint8_t>(value); }
};

template <>
struct coefficient_extractor<greyscalea, coefficient_type::alpha> {
    double operator()(const greyscalea& col) const { return colour_conv<double>(col.a); }
    void operator()(greyscalea& col, const double& value) const { col.a = colour_conv<uint8_t>(value); }
};

template <coefficient_type Coeff, typename Colour>
concept extractible_coefficient = yield_invocable<const coefficient_extractor<Colour, Coeff>&, double, const Colour&>;

template <coefficient_type Coeff, typename Colour>
concept editable_coefficient = yield_invocable<const coefficient_extractor<Colour, Coeff>&, void, Colour&, const double&>;

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

template <coefficient_type Coeff>
requires(detail::editable_coefficient<Coeff, rol::rgba>)
bool insert_coefficient_inplace(rgb_image& img, const coefficient_plane& coeffs, const math::vec2i& offset) {
    static constexpr detail::coefficient_extractor<rol::rgba, Coeff> extractor {};
    return overlayed_images_intersection(img, coeffs, offset, [](const math::vec2u& size, rgb_image& img, const math::vec2u& img_start, const coefficient_plane& coeffs, const math::vec2u& coeffs_start) {
        for(std::size_t y = 0; y < size.y; y++) {
            std::span<rgba> image_row = img[y + img_start.y].subspan(img_start.x);
            std::span<const double> coeff_row = coeffs[y + coeffs_start.y].subspan(coeffs_start.x);
            for(std::size_t x = 0; x < size.x; x++) {
                extractor(image_row[x], coeff_row[x]);
            }
        }
    });
}


/*--------------------+
|  GreyscaleA         |
+--------------------*/

template <coefficient_type Coeff>
requires(detail::extractible_coefficient<Coeff, rol::greyscalea>)
inline coefficient_plane extract_coefficient(const greyscale_image& img) {
    return map_pixels<double>(img, detail::coefficient_extractor<rol::greyscalea, Coeff> {});
}

template <coefficient_type Coeff>
requires(detail::editable_coefficient<Coeff, rol::greyscalea>)
bool insert_coefficient_inplace(greyscale_image& img, const coefficient_plane& coeffs, const math::vec2i& offset) {
    static constexpr detail::coefficient_extractor<rol::greyscalea, Coeff> extractor {};
    return overlayed_images_intersection(img, coeffs, offset, [](const math::vec2u& size, greyscale_image& img, const math::vec2u& img_start, const coefficient_plane& coeffs, const math::vec2u& coeffs_start) {
        for(std::size_t y = 0; y < size.y; y++) {
            std::span<greyscalea> image_row = img[y + img_start.y].subspan(img_start.x);
            std::span<const double> coeff_row = coeffs[y + coeffs_start.y].subspan(coeffs_start.x);
            for(std::size_t x = 0; x < size.x; x++) {
                extractor(image_row[x], coeff_row[x]);
            }
        }
    });
}

} // namespace rol::algo
