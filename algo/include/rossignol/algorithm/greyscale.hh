
#pragma once

#include "rossignol/image/binary_image.hh"
#include "rossignol/image/channel.hh"
#include "rossignol/image/coefficient_plane.hh"
#include "rossignol/image/greyscale_image.hh"
#include "rossignol/image/rgb_image.hh"
#include "rossignol/params/luma_method.hh"


namespace rol::algo {

template <luma_method Method = defaults::luma_method>
greyscale_image greyscale_luma(const rgb_image& img);

greyscale_image greyscale_intensity(const rgb_image& img);

greyscale_image greyscale_lightness(const rgb_image& img);

greyscale_image greyscale_value(const rgb_image& img);


greyscale_image greyscale(const binary_image& img);

greyscale_image greyscale(const layer& img);

greyscale_image greyscale(const coefficient_plane& img);

} // namespace rol::algo
