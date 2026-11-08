
#pragma once

#include <variant>

#include "rossignol/image/image.hh"


namespace rol::generic {

using image = std::variant<rgb_image, greyscale_image, binary_image, layer, coefficient_plane>;

} // namespace rol::generic
