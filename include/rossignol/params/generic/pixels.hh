
#pragma once

#include <cstdint>

#include <fmt/format.h>


namespace rol::generic {

struct pixel_count {
    int64_t value;

    pixel_count operator-() const {
        return pixel_count { .value = -value };
    }
};

} // namespace rol::generic


namespace rol::literals {

inline generic::pixel_count operator""_px(unsigned long long int val) {
    return generic::pixel_count {
        .value = static_cast<int64_t>(val)
    };
}

} // namespace rol::literals


template <>
struct fmt::formatter<rol::generic::pixel_count> {

    constexpr auto parse(fmt::format_parse_context& ctx) -> fmt::format_parse_context::iterator {
        if (ctx.begin() != ctx.end() && *ctx.begin() != '}') {
            throw fmt::format_error("invalid pixel count format");
        }
        return ctx.begin();
    }

    auto format(const rol::generic::pixel_count& size, fmt::format_context& ctx) const -> fmt::format_context::iterator {
        return fmt::format_to(ctx.out(), "{}px", size.value);
    }
};
