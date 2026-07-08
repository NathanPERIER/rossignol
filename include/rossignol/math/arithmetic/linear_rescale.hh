
#pragma once

#include <cstdint>


namespace rol::math {

inline double linear_rescale(double value, double from_min, double from_max, double to_min, double to_max) {
    if(value < from_min) [[unlikely]] {
        return to_min;
    }
    if(value > from_max) [[unlikely]] {
        return to_max;
    }
    return (value - from_min) / (from_max - from_min) * (to_max - to_min) + to_min;
}

inline uint8_t linear_rescale(uint8_t value, uint8_t from_min, uint8_t from_max, uint8_t to_min, uint8_t to_max) {
    if(value < from_min) [[unlikely]] {
        return to_min;
    }
    if(value > from_max) [[unlikely]] {
        return to_max;
    }
    return static_cast<uint8_t>(
        (static_cast<int32_t>(value) - static_cast<int32_t>(from_min)) * static_cast<int32_t>(to_max - to_min) / static_cast<int32_t>(from_max - from_min)
    ) + to_min;
}

} // namespace rol::math
