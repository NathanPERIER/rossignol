
#pragma once

#include "rossignol/image/image.hh"


namespace rol::generic {

class invert {
public:
    rgb_image operator()(const rgb_image& img) const;
    greyscale_image operator()(const greyscale_image& img) const;
    binary_image operator()(const binary_image& img) const;
    layer operator()(const layer& img) const;
    coefficient_plane operator()(const coefficient_plane& img) const;

    image operator()(const image& img) const;
};

} // namespace rol::generic
