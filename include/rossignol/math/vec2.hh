
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

} // namespace rol::math
