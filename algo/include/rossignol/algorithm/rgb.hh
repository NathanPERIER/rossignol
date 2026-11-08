
#pragma once

#include "rossignol/image/image.hh"


namespace rol::algo {

rgb_image to_rgb(const greyscale_image& img);

rgb_image to_rgb(const binary_image& img);

rgb_image to_rgb(const layer& img);

rgb_image to_rgb(const coefficient_plane& img);

} // namespace rol::algo
