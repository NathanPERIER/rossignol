
#pragma once

#include <variant>

#include "rossignol/colour/builtin.hh"
#include "rossignol/utils/typing/variant.hh"


namespace rol {

class colour {
public:
    using variant = std::variant<rgba, greyscalea, bool, uint8_t, double>;

    template <typename Colour>
    requires(variant_alternative<Colour, variant>)
    colour(const Colour& col): _col(col) {}

    colour(builtin_binary_colour col): _col(get_binary_colour(col)) {}
    colour(builtin_greyscale_colour col): _col(get_greyscale_colour(col)) {}
    colour(builtin_rgb_colour col): _col(get_rgb_colour(col)) {}

    template <typename Colour>
    requires(variant_alternative<Colour, variant>)
    Colour decay_to() const;

private:
    variant _col;
};

} // namespace rol
