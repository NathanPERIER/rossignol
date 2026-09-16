
#pragma once

#include "rossignol/image/image.hh"

#include "rossignol/params/generic/channel_value.hh"


namespace rol::generic {

class rescale_values {
public:
    rescale_values(): rescale_values(defaults::channel_min, defaults::channel_max) {}
    rescale_values(channel_value min, channel_value max): _min(min), _max(max) {}

    template <rol::any_of<uint8_t, double> Colour>
    rol::basic_image<Colour> operator()(const rol::basic_image<Colour>& img) const;

    image operator()(const image& img) const;

private:
    channel_value _min;
    channel_value _max;

    image operator()(const rgb_image&) const;
    image operator()(const greyscale_image&) const;
    image operator()(const binary_image&) const;
};

} // namespace rol::generic
