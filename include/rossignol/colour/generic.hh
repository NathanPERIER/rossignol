
#pragma once

#include <variant>

#include "rossignol/colour/builtin.hh"
#include "rossignol/utils/typing/variant.hh"


namespace rol {

using colour = std::variant<rgba, greyscalea, bool, uint8_t, double>;

colour get_colour(builtin_rgb_colour col) {
    return get_rgb_colour(col);
}

colour get_colour(builtin_greyscale_colour col) {
    return get_greyscale_colour(col);
}

colour get_colour(builtin_binary_colour col) {
    return get_binary_colour(col);
}


template <typename T>
requires(variant_alternative<T, colour>)
T decay_colour(const colour& col);

} // namespace rol
