
#pragma once

#include <variant>

#include <rossignol/math/vec2.hh>
#include <rossignol/params/alignment.hh>
#include <rossignol/params/generic/percentage.hh>
#include <rossignol/params/generic/pixels.hh>


namespace rol::generic {

using horizontal_offset = std::variant<pixel_count, percentage, horizontal_alignment>;
using vertical_offset = std::variant<pixel_count, percentage, vertical_alignment>;

math::vec2i compute_offset(horizontal_offset x, vertical_offset y, math::vec2u object_size, math::vec2u reference_size);

} // namespace rol::generic


namespace rol::generic::defaults {

constexpr rol::generic::horizontal_offset horizontal_offset = rol::defaults::alignment.x;
constexpr rol::generic::vertical_offset vertical_offset = rol::defaults::alignment.y;

} // namespace rol::generic::defaults
