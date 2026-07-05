
#pragma once


namespace rol::generic {

enum class cylindrical_colour_type {
    hsi,
    hsl,
    hsv
};

} // namespace rol::generic


namespace rol::generic::defaults {

constexpr rol::generic::cylindrical_colour_type cylindrical_colour_type = rol::generic::cylindrical_colour_type::hsv;

} // namespace rol::generic::defaults
