
#pragma once

#include "rossignol/algorithm/map_pixels.hh"
#include "rossignol/image/image.hh"


namespace rol::algo {

template <std::size_t N>
requires (0 <= N && N < 8)
binary_image extract_bit_plane(const layer& layr) {
    constexpr uint8_t mask = 0b00000001 << N;
    return map_pixels<bool>(layr, [](const uint8_t& pixel) {
        return (pixel & mask) != 0;
    });
}

} // namespace rol::algo

