
#pragma once

#include "rossignol/image/binary_image.hh"
#include "rossignol/image/channel.hh"
#include "rossignol/image/coefficient_plane.hh"
#include "rossignol/image/greyscale_image.hh"
#include "rossignol/image/rgb_image.hh"


namespace rol::algo {

rgb_image to_rgb(const greyscale_image& img);

rgb_image to_rgb(const binary_image& img);

rgb_image to_rgb(const layer& img);

rgb_image to_rgb(const coefficient_plane& img);

} // namespace rol::algo
