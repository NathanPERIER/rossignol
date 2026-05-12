
#pragma once

#include "rossignol/math/rect.hh"
#include "rossignol/params/alignment.hh"


namespace rol::math {

template <horizontal_alignment AlignX, vertical_alignment AlignY>
rect align(const vec2u& size, const rect& reference) {
    vec2i p1;
	vec2i p2;
	if constexpr (AlignX == horizontal_alignment::left) {
		p1.x = reference.x1();
		p2.x = p1.x + static_cast<int64_t>(size.x);
	} else if constexpr (AlignX == horizontal_alignment::right) {
		p2.x = reference.x2();
		p1.x = p2.x - static_cast<int64_t>(size.x);
	} else {
		static_assert(AlignX == horizontal_alignment::centre);
		p1.x = (static_cast<int64_t>(size.x) - static_cast<int64_t>(reference.width())) / 2;
		p2.x = p1.x + static_cast<int64_t>(size.x);
	}
	if constexpr (AlignY == vertical_alignment::top) {
		p1.y = reference.y1();
		p2.y = p1.y + static_cast<int64_t>(size.y);
	} else if constexpr (AlignY == vertical_alignment::bottom) {
		p2.y = reference.y2();
		p1.y = p2.y - static_cast<int64_t>(size.y);
	} else {
		static_assert(AlignY == vertical_alignment::centre);
		p1.y = (static_cast<int64_t>(size.y) - static_cast<int64_t>(reference.height())) / 2;
		p2.y = p1.y + static_cast<int64_t>(size.y);
	}
    return rect(p1, p2);
}

} // namespace rol::math
