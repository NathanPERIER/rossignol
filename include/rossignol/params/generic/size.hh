
#pragma once

#include <variant>

#include <rossignol/math/vec2.hh>
#include <rossignol/params/generic/percentage.hh>
#include <rossignol/params/generic/pixels.hh>


namespace rol::generic {

using image_length = std::variant<pixel_count, percentage>;

math::vec2u compute_size(image_length x, image_length y, math::vec2u image_size);

} // namespace rol::generic
