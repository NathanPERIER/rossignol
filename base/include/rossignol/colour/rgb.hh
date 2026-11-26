
#pragma once

#include <fmt/format.h>

#include "rossignol/utils/parsing/hex.hh"


namespace rol {

struct rgb {
    uint8_t r;
    uint8_t g;
    uint8_t b;

    bool operator==(const rgb& col) const = default;
};

struct rgba {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a = 255;

    bool operator==(const rgba& col) const = default;
};

} // namespace rol


namespace rol::literals {

constexpr rol::rgb operator""_rgb(const char* repr, std::size_t size) {
    if(size != 7) {
        throw std::runtime_error("RGB colour must be 7 characters long");
    }
    if(repr[0] != '#') {
        throw std::runtime_error("RGB colour must start with a '#'");
    }
    rol::rgb res;
    res.r = parse_hex_byte(repr[1], repr[2]);
    res.g = parse_hex_byte(repr[3], repr[4]);
    res.b = parse_hex_byte(repr[5], repr[6]);
    return res;
}

constexpr rol::rgba operator""_rgba(const char* repr, std::size_t size) {
    if(size != 7 && size != 9) {
        throw std::runtime_error("RGBA colour must be eiher 7 or 9 characters long");
    }
    if(repr[0] != '#') {
        throw std::runtime_error("RGBA colour must start with a '#'");
    }
    rol::rgba res;
    res.r = parse_hex_byte(repr[1], repr[2]);
    res.g = parse_hex_byte(repr[3], repr[4]);
    res.b = parse_hex_byte(repr[5], repr[6]);
    res.a = (size == 9) ? parse_hex_byte(repr[7], repr[8]) : 255;
    return res;
}

} // namespace rol::literals


template <>
struct fmt::formatter<rol::rgb> {

    constexpr auto parse(fmt::format_parse_context& ctx) -> fmt::format_parse_context::iterator {
        if (ctx.begin() != ctx.end() && *ctx.begin() != '}') {
            throw fmt::format_error("invalid RGB colour format");
        }
        return ctx.begin();
    }

    auto format(const rol::rgb& col, fmt::format_context& ctx) const -> fmt::format_context::iterator {
        return fmt::format_to(ctx.out(), "#{:02x}{:02x}{:02x}", col.r, col.g, col.b);
    }
};

template <>
struct fmt::formatter<rol::rgba> {

    constexpr auto parse(fmt::format_parse_context& ctx) -> fmt::format_parse_context::iterator {
        if (ctx.begin() != ctx.end() && *ctx.begin() != '}') {
            throw fmt::format_error("invalid RGBA colour format");
        }
        return ctx.begin();
    }

    auto format(const rol::rgba& col, fmt::format_context& ctx) const -> fmt::format_context::iterator {
        return fmt::format_to(ctx.out(), "#{:02x}{:02x}{:02x}{:02x}", col.r, col.g, col.b, col.a);
    }
};
