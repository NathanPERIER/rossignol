
#pragma once

#include <concepts>


namespace rol::math {

template <std::unsigned_integral Integer>
requires(!std::same_as<Integer, bool>)
Integer average(Integer i1, Integer i2) {
    // This implementation uses the property of addition where binary numbers that don't
    // have common bits can be added by simply using the binary OR operand
    // For instance :
    //     3         0011
    //  +  8  <=>  + 1000
    //  = 11       = 1011

    // First, we find all the bits in common between our two numbers
    const Integer c = i1 & i2;
    // Then, we compute `a` and `b`, which are our numbers without the common bits
    const Integer a = i1 & ~c;
    const Integer b = i2 & ~c;
    // We know that :
    //   - i1 = a | c = a + c (because `a` and `c` have no common bits)
    //   - i2 = b | c = b + c (because `b` and `c` have no common bits)
    // It follows that the average is :
    //    (i1 + i2) / 2
    //  = (a + c + b + c) / 2
    //  = c + (a + b) / 2
    //  = c + (a | b) / 2      (because `a` and `b` have no common bits)
    return c + (a | b) / 2;
}

template <std::floating_point Float>
Float average(Float f1, Float f2) {
    return (f1 + f2) / 2.0;
}

} // namespace rol::math
