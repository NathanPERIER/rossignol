
#pragma once

#include <variant>

#include "rossignol/image/binary_image.hh"
#include "rossignol/image/channel.hh"
#include "rossignol/image/coefficient_plane.hh"
#include "rossignol/image/greyscale_image.hh"
#include "rossignol/image/rgb_image.hh"


namespace rol::generic {

using image = std::variant<rgb_image, greyscale_image, binary_image, layer, coefficient_plane>;

} // namespace rol::generic
