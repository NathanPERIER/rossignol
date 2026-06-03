
#pragma once

#include <cstdint>


namespace rol::math {

inline uint8_t linear_interpolation(uint8_t val1, uint8_t dist1, uint8_t val2, uint8_t dist2) {
    const uint16_t val1_contribution = static_cast<uint16_t>(val1) * static_cast<uint16_t>(dist2);
    const uint16_t val2_contribution = static_cast<uint16_t>(val2) * static_cast<uint16_t>(dist1);
    return static_cast<uint8_t>((val1_contribution + val2_contribution) / (dist1 + dist2));
}

inline uint8_t linear_interpolation(uint8_t val1, double dist1, uint8_t val2, double dist2) {
    const double val1_contribution = static_cast<double>(val1) * dist2;
    const double val2_contribution = static_cast<double>(val2) * dist1;
    return static_cast<uint8_t>((val1_contribution + val2_contribution) / (dist1 + dist2));
}

} // namespace rol::math
