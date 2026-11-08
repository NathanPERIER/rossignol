
#pragma once

#include <fmt/format.h>


namespace rol {

struct hsv {
    double h;
    double s;
    double v;

    bool operator==(const hsv& col) const = default;
};

} // namespace rol


template <>
struct fmt::formatter<rol::hsv> {

    constexpr auto parse(fmt::format_parse_context& ctx) -> fmt::format_parse_context::iterator {
        if (ctx.begin() != ctx.end() && *ctx.begin() != '}') {
            throw fmt::format_error("invalid HSV colour format");
        }
        return ctx.begin();
    }

    auto format(const rol::hsv& col, fmt::format_context& ctx) const -> fmt::format_context::iterator {
        return fmt::format_to(ctx.out(), "hsv({:.2f}, {:.2f}, {:.2f})", col.h, col.s, col.v);
    }
};
