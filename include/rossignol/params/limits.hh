
#pragma once

#include <cstdint>
#include <numeric>

#include "rossignol/utils/typing/any_of.hh"


namespace rol::defaults {

constexpr uint8_t min_u8 = std::numeric_limits<uint8_t>::min();
constexpr uint8_t max_u8 = 255;

template <any_of<uint8_t, double> Colour>
struct channel_limits;

template <>
struct channel_limits<uint8_t> {
    static constexpr uint8_t min = std::numeric_limits<uint8_t>::min();
    static constexpr uint8_t max = std::numeric_limits<uint8_t>::max();
};

template <>
struct channel_limits<double> {
    static constexpr double min = 0.0;
    static constexpr double max = 1.0;
};

} // namespace rol::defaults
