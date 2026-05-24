
#pragma once

#include <variant>

#include "rossignol/colour/rgb.hh"
#include "rossignol/colour/greyscale.hh"
#include "rossignol/utils/typing/variant.hh"


namespace rol {

using colour = std::variant<rgba, greyscalea, bool, uint8_t, double>;

template <typename T>
requires(variant_alternative<T, colour>)
T decay_colour(const colour& col);

} // namespace rol
