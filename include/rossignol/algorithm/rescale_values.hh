
#pragma once

#include <optional>

#include "rossignol/algorithm/map_pixels.hh"
#include "rossignol/math/arithmetic/linear_rescale.hh"
#include "rossignol/utils/typing/any_of.hh"


namespace rol::algo::detail {

template <typename Colour>
requires(rol::any_of<Colour, uint8_t, double>)
inline void rescale_values_inplace_impl(rol::basic_image<Colour>& img, const std::optional<Colour>& min, const std::optional<Colour>& max) {
    if(!min.has_value() && !max.has_value()) {
        throw std::runtime_error("At least the minimum or the maximum should be specified for a rescale operation");
    }

    if(min.has_value() && max.has_value() && max.value() < min.value()) {
        throw std::runtime_error("The minimum for a rescale operation should always be smaller than or equal to the maximum");
    }

    Colour pixels_min = std::numeric_limits<Colour>::max();
    Colour pixels_max = std::numeric_limits<Colour>::min();
    edit_pixels_inplace(img, [&pixels_min, &pixels_max](const Colour& pixel) {
        if(pixel > pixels_max) {
            pixels_max = pixel;
        }
        if(pixel < pixels_min) {
            pixels_min = pixel;
        }
    });

    Colour to_min = min.value_or(pixels_min);
    Colour to_max = max.value_or(pixels_max);

    // At this point, it is possible that the minimum be greater than the maximum iff one of those was not specified
    // In this case, the operation is equivalent to squashing all values to the extremum that was specified
    if(to_max < to_min) {
        // TODO warning ?
        if(min.has_value()) {
            // The minimum was specified (but not the maximum)
            to_max = min.value();
        } else {
            // The maximum was specified (but not the minimum)
            to_min = max.value();
        }
    }

    // This handles both of the following cases :
    //  - All the pixels in the image have the same value
    //  - The maximum and minimum in the destination range have the same value (see previous section)
    if((pixels_max - pixels_min) <= std::numeric_limits<Colour>::epsilon() || (to_max - to_min) <= std::numeric_limits<Colour>::epsilon()) {
        // We set all the pixels to the same value, which is the average of the min and the max
        const Colour value = (to_max + to_min) / 2;
        edit_pixels_inplace(img, [&value](Colour& pixel) { pixel = value; });
        return;
    }

    edit_pixels_inplace(img, [&pixels_min, &pixels_max, &to_min, &to_max](Colour& pixel) {
        pixel = math::linear_rescale(pixel, pixels_min, pixels_max, to_min, to_max);
    });
}

} // namespace rol::algo::detail


namespace rol::algo {

inline void rescale_values_inplace(layer& img, std::optional<uint8_t> min, std::optional<uint8_t> max) {
    detail::rescale_values_inplace_impl(img, min, max);
}

inline void rescale_values_inplace(coefficient_plane& img, std::optional<double> min, std::optional<double> max) {
    if(min.has_value() && min.value() < 0.0) {
        throw std::runtime_error("Minimum for rescaling must be greater than or equal to zero");
    }
    if(max.has_value() && max.value() > 1.0) {
        throw std::runtime_error("Maximum for rescaling must be smaller than or equal to one");
    }
    detail::rescale_values_inplace_impl(img, min, max);
}

} // namespace rol::algo
