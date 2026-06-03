#include "rossignol/colour/gradient.hh"

namespace {

template <typename W>
rgba rgb_interpolation(const rgba& col1, W weight1, const rgba& col2, W weight2, W value) {
    if(value <= weight1) {
        return col1;
    }
    if(value >= weight2) {
        return col2;
    }
    const W dist1 = value - weight1;
    const W dist2 = weight2 - value;
    return rgba {
        .r = math::linear_interpolation(col1.r, dist1, col2.r, dist2),
        .g = math::linear_interpolation(col1.g, dist1, col2.g, dist2),
        .b = math::linear_interpolation(col1.b, dist1, col2.b, dist2),
        .a = math::linear_interpolation(col1.a, dist1, col2.a, dist2)
    };
}


} // anonymous namespace

namespace rol {

rgba colour_mix(const rgba& col1, double weight1, const rgba& col2) {
    return ::rgb_interpolation(col1, 0.0, col2, 1.0, weight1);
}

} // namespace rol