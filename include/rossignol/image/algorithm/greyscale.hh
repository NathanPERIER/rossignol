
#pragma once

#include "rossignol/image/image.hh"
#include "rossignol/colour/algorithms/luma.hh"


namespace rol::defaults {

constexpr luma_method greyscale_method = luma_method::sdtv;

} // namespace rol::defaults


namespace rol::detail {

template <luma_method Method = defaults::greyscale_method>
greyscale_image greyscale(const rgb_image& img);

greyscale_image greyscale(const binary_image& img);

greyscale_image greyscale(const layer& img);

greyscale_image greyscale(const coefficient_plane& img);

} // namespace rol::detail


namespace rol {

greyscale_image to_greyscale(const image& img, luma_method method = defaults::greyscale_method);

} // namespace rol
