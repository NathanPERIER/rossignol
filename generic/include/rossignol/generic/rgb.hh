
#pragma once

#include "rossignol/image/image.hh"


namespace rol::generic {

class to_rgb {
public:
    rgb_image operator()(const rgb_image& img) const { return img.share(); }
    rgb_image operator()(const greyscale_image& img) const;
    rgb_image operator()(const binary_image& img) const;
    rgb_image operator()(const layer& img) const;
    rgb_image operator()(const coefficient_plane& img) const;

    rgb_image operator()(const image& img) const;
};

} // namespace rol::generic
