
#pragma once

#include <cstdint>
#include <stdexcept>


namespace rol {

constexpr uint8_t parse_hex_nibble(const char& c) {
    if('0' <= c && c <= '9') {
        return static_cast<uint8_t>(c - '0');
    }
    if('a' <= c && c <= 'f') {
        return static_cast<uint8_t>(c - 'a' + 10);
    }
    if('A' <= c && c <= 'F') {
        return static_cast<uint8_t>(c - 'A' + 10);
    }
    throw std::runtime_error("Bad hex character");
}

constexpr uint8_t parse_hex_byte(const char& high, const char& low) {
    return (parse_hex_nibble(high) << 4) | parse_hex_nibble(low);
}

} // namespace rol