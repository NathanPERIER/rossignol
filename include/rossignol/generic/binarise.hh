
#pragma once

#include "rossignol/image/image.hh"
#include "rossignol/params/binarise.hh"


namespace rol::generic {

struct floyd_steinberg_t {};
constexpr floyd_steinberg_t floyd_steinberg;

struct binary_threshold {
    uint8_t value = defaults::binary_threshold;
};

using binarise_method = std::variant<floyd_steinberg_t, binary_threshold>;

class binarise {
public:
    binarise(binarise_method method): _method(method) {}

    binary_image operator()(const greyscale_image& img) const;
    binary_image operator()(const binary_image& img) const;

    binary_image operator()(const image& img) const;

private:
    binarise_method _method;
};

} // namespace rol::generic
