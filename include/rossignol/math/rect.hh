
#pragma once

#include <algorithm>

#include "rossignol/math/vec2.hh"


namespace rol::math {

struct rect {
public:
    using vec2_type = vec2i;
    using number_type = vec2_type::number_type;

    rect() = default;
    rect(const vec2_type& p1, const vec2_type& p2): _p1(p1), _p2(p2) {
        if(_p2.x < p1.x) {
            std::swap(_p2.x, _p1.x);
        }
        if(_p2.y < p1.y) {
            std::swap(_p2.y, _p1.y);
        }
    }
    rect(number_type x1, number_type y1, number_type x2, number_type y2): rect(vec2_type{ .x = x1, .y = y1 }, vec2_type{ .x = x2, .y = y2 }) {}
    explicit rect(const vec2u& size): rect(0, 0, static_cast<int64_t>(size.x), static_cast<int64_t>(size.y)) {}

    number_type x1() const { return _p1.x; };
    number_type x2() const { return _p2.x; };
    number_type y1() const { return _p1.y; };
    number_type y2() const { return _p2.y; };

    vec2u size() const {
        return vec2u {
            .x = static_cast<std::size_t>(_p2.x - _p1.x),
            .y = static_cast<std::size_t>(_p2.y - _p1.y)
        };
    }

    std::size_t width() const { return static_cast<std::size_t>(_p2.x - _p1.x); }
    std::size_t height() const { return static_cast<std::size_t>(_p2.y - _p1.y); }

    bool empty() const {
        return (_p1.x == _p2.x || _p1.y == _p2.y);
    }

    rect intersect(const rect& r) const {
	    return rect(std::max(x1(), r.x1()), std::max(y1(), r.y1()), std::min(x2(), r.x2()), std::min(y2(), r.y2()));
    }

    rect encompass(const rect& r) const {
	    return rect(std::min(x1(), r.x1()), std::min(y1(), r.y1()), std::max(x2(), r.x2()), std::max(y2(), r.y2()));
    }

    rect& operator+=(const vec2_type& offset) {
        _p1 += offset;
        _p2 += offset;
        return *this;
    }

    rect operator+(const vec2_type& offset) const {
        rect res(*this);
        res += offset;
        return res;
    }

    rect& operator-=(const vec2_type& offset) {
        _p1 -= offset;
        _p2 -= offset;
        return *this;
    }

    rect operator-(const vec2_type& offset) const {
        rect res(*this);
        res -= offset;
        return res;
    }

private:
	vec2_type _p1;
	vec2_type _p2;
};

} // namespace rol::math
