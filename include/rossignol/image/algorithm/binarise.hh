
#pragma once

#include "rossignol/image/image.hh"


namespace rol::defaults {

constexpr uint8_t binary_threshold = 127;

} // namespace rol::defaults


namespace rol::detail {

binary_image floyd_steinberg(const greyscale_image& img);

binary_image binarise_threshold(const greyscale_image& img, uint8_t threshold = defaults::binary_threshold);

} // namespace rol::detail


namespace rol {

struct floyd_steinberg_t {};
constexpr floyd_steinberg_t floyd_steinberg;

struct binary_threshold {
    uint8_t value = defaults::binary_threshold;
};

using binarise_method = std::variant<floyd_steinberg_t, binary_threshold>;

binary_image binarise(const image& img, binarise_method method);

} // namespace rol
