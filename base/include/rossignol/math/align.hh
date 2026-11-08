
#pragma once

#include "rossignol/math/rect.hh"
#include "rossignol/params/alignment.hh"


namespace rol::math {

template <horizontal_alignment AlignX>
int64_t aligned_horizontal_offset(std::size_t size, std::size_t reference_size) {
    if constexpr (AlignX == horizontal_alignment::left) {
        return 0;
    } else if constexpr (AlignX == horizontal_alignment::right) {
        return static_cast<int64_t>(reference_size) - static_cast<int64_t>(size);
    } else {
        static_assert(AlignX == horizontal_alignment::centre);
        return (static_cast<int64_t>(reference_size) - static_cast<int64_t>(size)) / 2;
    }
}

template <vertical_alignment AlignY>
int64_t aligned_vertical_offset(std::size_t size, std::size_t reference_size) {
    if constexpr (AlignY == vertical_alignment::top) {
        return 0;
    } else if constexpr (AlignY == vertical_alignment::bottom) {
        return static_cast<int64_t>(reference_size) - static_cast<int64_t>(size);
    } else {
        static_assert(AlignY == vertical_alignment::centre);
        return (static_cast<int64_t>(reference_size) - static_cast<int64_t>(size)) / 2;
    }
}

} // namespace rol::math
