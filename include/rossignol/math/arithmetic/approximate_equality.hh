
#pragma once

#include <cmath>
#include <concepts>
#include <numeric>


namespace rol::math {

template <std::integral Integer>
requires(!std::same_as<Integer, bool>)
Integer approx_equal(Integer i1, Integer i2) {
    return (i1 == i2);
}

template <std::floating_point Float>
Float approx_equal(Float f1, Float f2) {
    return std::fabs(f2 - f1) < std::numeric_limits<Float>::epsilon();
}

} // namespace rol::math
