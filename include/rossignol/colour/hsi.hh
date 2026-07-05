
#pragma once

#include <fmt/format.h>


namespace rol {

struct hsi {
    double h;
    double s;
    double i;

    bool operator==(const hsi& col) const = default;
};

} // namespace rol


template <>
struct fmt::formatter<rol::hsi> {

    constexpr auto parse(fmt::format_parse_context& ctx) -> fmt::format_parse_context::iterator {
        if (ctx.begin() != ctx.end() && *ctx.begin() != '}') {
            throw fmt::format_error("invalid HSI colour format");
        }
        return ctx.begin();
    }

    auto format(const rol::hsi& col, fmt::format_context& ctx) const -> fmt::format_context::iterator {
        return fmt::format_to(ctx.out(), "hsi({:.2f}, {:.2f}, {:.2f})", col.h, col.s, col.i);
    }
};
