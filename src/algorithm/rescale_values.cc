#include "rossignol/algorithm/rescale_values.hh"

#include "rossignol/math/arithmetic/approximate_equality.hh"
#include "rossignol/math/arithmetic/average.hh"
#include "rossignol/utils/typing/any_of.hh"

namespace {

template <rol::any_of<uint8_t, double> Colour>
void apply_rescaling(rol::basic_image<Colour>& img, const Colour& from_min, const Colour& from_max, const Colour& to_min, const Colour& to_max);

template <>
void apply_rescaling<uint8_t>(rol::layer& img, const uint8_t& from_min, const uint8_t& from_max, const uint8_t& to_min, const uint8_t& to_max) {
    // Unfortunately it seems we have to cast to 16-bit integers to avoid overflows
    const uint16_t scaling_den = static_cast<uint16_t>(from_max - from_min);
    if(to_min <= to_max) {
        const uint16_t scaling_num = static_cast<uint16_t>(to_max - to_min);
        for(std::size_t y = 0; y < img.height(); y++) {
            std::ranges::for_each(img[y], [&scaling_num, &scaling_den, &from_min, &to_min](uint8_t& pixel) {
                pixel = static_cast<uint8_t>((static_cast<uint16_t>(pixel - from_min) * scaling_num) / scaling_den) + to_min;
            });
        }
    } else {
        // We invert the signs to keep everything positive and avoid having to switch to signed integers
        const uint16_t scaling_num = static_cast<uint16_t>(to_min - to_max);
        for(std::size_t y = 0; y < img.height(); y++) {
            std::ranges::for_each(img[y], [&scaling_num, &scaling_den, &from_min, &to_min](uint8_t& pixel) {
                pixel = to_min - static_cast<uint8_t>((static_cast<uint16_t>(pixel - from_min) * scaling_num) / scaling_den);
            });
        }
    }
}

template <>
void apply_rescaling<double>(rol::coefficient_plane& img, const double& from_min, const double& from_max, const double& to_min, const double& to_max) {
    const double scaling = (to_max - to_min) / (from_max - from_min);
    for(std::size_t y = 0; y < img.height(); y++) {
        std::ranges::for_each(img[y], [&scaling, &from_min, &to_min](double& pixel) {
            pixel = ((pixel - from_min) * scaling) + to_min;
        });
    }
}


template <rol::any_of<uint8_t, double> Colour>
void rescale_values_inplace_impl(rol::basic_image<Colour>& img, const Colour& min, const Colour& max) {
    if(rol::math::approx_equal(min, max)) {
        // Should we throw an error here ?
        for(std::size_t y = 0; y < img.height(); y++) {
            std::ranges::fill(img[y], min);
        }
    }
    const auto initial_range_minmax = std::ranges::minmax_element(img[0]);
    Colour initial_min = *initial_range_minmax.min;
    Colour initial_max = *initial_range_minmax.max;
    for(std::size_t y = 1; y < img.height(); y++) {
        const auto row_minmax = std::ranges::minmax_element(img[y]);
        initial_min = std::min(initial_min, *row_minmax.min);
        initial_max = std::max(initial_max, *row_minmax.max);
    }
    if(rol::math::approx_equal(initial_min, initial_max)) {
        const Colour value = rol::math::average(min, max);
        for(std::size_t y = 0; y < img.height(); y++) {
            std::ranges::fill(img[y], value);
        }
        return;
    }
    apply_rescaling<Colour>(img, initial_min, initial_max, min, max);
}

} // anonymous namespace


namespace rol::algo {

void rescale_values_inplace(layer& img, uint8_t min, uint8_t max) {
    ::rescale_values_inplace_impl(img, min, max);
}

void rescale_values_inplace(coefficient_plane& img, double min, double max) {
    ::rescale_values_inplace_impl(img, min, max);
}

} // namespace rol::algo
