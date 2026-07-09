
#pragma once

#include <cstdint>

#include "rossignol/utils/typing/any_of.hh"


namespace rol::math {

template <typename Number>
struct vec2 {
    using number_type = Number;
    Number x;
    Number y;

    vec2<Number>& operator+=(const vec2<Number>& offset) {
        x += offset.x;
        y += offset.y;
        return *this;
    }

    vec2<Number> operator+(const vec2<Number>& offset) const {
        vec2<Number> res(*this);
        res += offset;
        return res;
    }
};

using vec2i = vec2<int64_t>;
using vec2u = vec2<std::size_t>;
using vec2f = vec2<double>;

template <typename Number>
requires(any_of<Number, int64_t, double>)
inline vec2<Number>& operator-=(vec2<Number>& v1, const vec2<Number>& v2) {
    v1.x -= v2.x;
    v1.y -= v2.y;
    return v1;
}

template <typename Number>
requires(any_of<Number, int64_t, double>)
inline vec2<Number> operator-(const vec2<Number>& v1, const vec2<Number>& v2) {
    vec2<Number> res = v1;
    res -= v2;
    return res;
}

template <typename Number>
requires(any_of<Number, int64_t, double>)
inline vec2<Number> operator-(const vec2<Number>& v) {
    return vec2<Number> {
        .x = -v.x,
        .y = -v.y
    };
}

template <typename Number>
requires(any_of<Number, int64_t, double>)
inline Number operator*(const vec2<Number>& v1, const vec2<Number>& v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

} // namespace rol::math
