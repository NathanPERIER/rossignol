
#pragma once

#include <fmt/format.h>


namespace rol {

struct hsl {
    double h;
    double s;
    double l;

    bool operator==(const hsl& col) const = default;
};

} // namespace rol


template <>
struct fmt::formatter<rol::hsl> {

    constexpr auto parse(fmt::format_parse_context& ctx) -> fmt::format_parse_context::iterator {
        if (ctx.begin() != ctx.end() && *ctx.begin() != '}') {
            throw fmt::format_error("invalid HSL colour format");
        }
        return ctx.begin();
    }

    auto format(const rol::hsl& col, fmt::format_context& ctx) const -> fmt::format_context::iterator {
        return fmt::format_to(ctx.out(), "hsl({:.2f}, {:.2f}, {:.2f})", col.h, col.s, col.l);
    }
};
