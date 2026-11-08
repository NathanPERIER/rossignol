
#pragma once

#include <cstdint>


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

inline vec2i& operator-=(vec2i& v1, const vec2i& v2) {
    v1.x -= v2.x;
    v1.y -= v2.y;
    return v1;
}

inline vec2i operator-(const vec2i& v1, const vec2i& v2) {
    vec2i res = v1;
    res -= v2;
    return res;
}

inline vec2i operator-(const vec2i& v) {
    return vec2i {
        .x = -v.x,
        .y = -v.y
    };
}

} // namespace rol::math
