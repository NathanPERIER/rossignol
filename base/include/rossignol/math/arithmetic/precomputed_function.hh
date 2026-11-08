
#pragma once

#include <array>

#include "rossignol/utils/typing/functional.hh"


namespace rol::math {

template <typename T>
class precomputed_u8_function {
public:
    template <typename F>
    requires(yield_invocable<F, T, uint8_t>)
    precomputed_u8_function(const F& func) {
        for(std::size_t i = 0; i < 256; i++) {
            _values[i] = func(static_cast<uint8_t>(i));
        }
    }

    const T& operator()(uint8_t i) const {
        return _values[static_cast<std::size_t>(i)];
    }

private:
    std::array<T, 256> _values;
};

} // namespace rol::math
