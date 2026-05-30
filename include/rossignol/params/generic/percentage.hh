
#pragma once

#include <cstdint>

#include <fmt/format.h>


namespace rol::generic {

struct percentage {
    double value;

    std::size_t apply(std::size_t v) const {
        return static_cast<std::size_t>(static_cast<double>(v) * value);
    }

    percentage operator-() const {
        return percentage { .value = -value };
    }
};

} // namespace rol::generic


namespace rol::literals {

inline generic::percentage operator""_pct(unsigned long long int val) {
    return generic::percentage {
        .value = static_cast<double>(val) / 100.0
    };
}

inline generic::percentage operator""_pct(long double val) {
    return generic::percentage {
        .value = static_cast<double>(val) / 100.0
    };
}

} // namespace rol::literals


template <>
struct fmt::formatter<rol::generic::percentage> {

    constexpr auto parse(fmt::format_parse_context& ctx) -> fmt::format_parse_context::iterator {
        if (ctx.begin() != ctx.end() && *ctx.begin() != '}') {
            throw fmt::format_error("invalid percentage format");
        }
        return ctx.begin();
    }

    auto format(const rol::generic::percentage& pct, fmt::format_context& ctx) const -> fmt::format_context::iterator {
        return fmt::format_to(ctx.out(), "{:.2f}%", pct.value * 100.0);
    }
};
