
#pragma once

#include "rossignol/image/image.hh"


namespace rol {

enum class layer_name {
    red,
    green,
    blue,
    grey,
    alpha
};

} // namespace rol


namespace rol::detail {

template<layer_name Layer>
requires (Layer == layer_name::red || Layer == layer_name::green || Layer == layer_name::blue || Layer == layer_name::alpha)
layer extract_channel(const rgb_image& img);

template<layer_name Layer>
requires (Layer == layer_name::grey || Layer == layer_name::alpha)
layer extract_channel(const greyscale_image& img);

layer extract_channel(const binary_image& img);

} // namespace rol::detail


namespace rol {

layer extract_channel(const image& img, layer_name layr);

} // namespace rol
