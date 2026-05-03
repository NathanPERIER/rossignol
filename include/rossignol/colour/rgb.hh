
#pragma once

#include <cstdint>
#include <string_view>

#include <fmt/format.h>


namespace rol {

struct rgba {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a = 255;

    double hue() const;
    double chroma() const;
    double hsl_saturation() const;
    double hsv_saturation() const;
    double hsi_saturation() const;
    uint8_t lightness() const;
    uint8_t value() const;
    uint8_t intensity() const;
    double lightness_coeff() const;
    double value_coeff() const;
    double intensity_coeff() const;

    bool operator==(const rgba& col) const = default;
};

} // namespace rol


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
