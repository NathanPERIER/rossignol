
#pragma once

#include "rossignol/image/image.hh"
#include "rossignol/params/layer_name.hh"


namespace rol::algo {

template<layer_name Layer>
requires (Layer == layer_name::red || Layer == layer_name::green || Layer == layer_name::blue || Layer == layer_name::alpha)
layer extract_channel(const rgb_image& img);

template<layer_name Layer>
requires (Layer == layer_name::grey || Layer == layer_name::alpha)
layer extract_channel(const greyscale_image& img);

layer extract_channel(const binary_image& img);

layer to_channel(const coefficient_plane& img);

} // namespace rol::algo
