
#pragma once

#include "rossignol/colour/greyscale.hh"
#include "rossignol/colour/rgb.hh"
#include "rossignol/utils/typing/any_of.hh"


namespace rol {

template <typename DestColour>
requires(any_of<DestColour, rgba, greyscalea, double>)
inline DestColour colour_conv(uint8_t col);

template <>
inline greyscalea colour_conv<greyscalea>(uint8_t col) {
    return greyscalea {
        .grey = col,
        .a = 255
    };
}

template <>
inline rgba colour_conv<rgba>(uint8_t col) {
    return rgba {
        .r = col,
        .g = col,
        .b = col,
        .a = 255
    };
}

template <>
inline double colour_conv<double>(uint8_t col) {
    return static_cast<double>(col) / 255.0;
}


template <typename DestColour>
requires(any_of<DestColour, rgba, greyscalea, uint8_t, double>)
inline DestColour colour_conv(bool col);

template <>
inline double colour_conv<double>(bool col) {
    return col ? 255.0 : 0.0;
}

template <>
inline uint8_t colour_conv<uint8_t>(bool col) {
    return col ? 255 : 0;
}

template <>
inline greyscalea colour_conv<greyscalea>(bool col) {
    return colour_conv<greyscalea>(colour_conv<uint8_t>(col));
}

template <>
inline rgba colour_conv<rgba>(bool col) {
    return colour_conv<rgba>(colour_conv<uint8_t>(col));
}



template <typename DestColour>
requires(any_of<DestColour, rgba, greyscalea, uint8_t>)
inline DestColour colour_conv(double col);

template <>
inline uint8_t colour_conv<uint8_t>(double col) {
    return static_cast<uint8_t>(col * 255.0);
}

template <>
inline greyscalea colour_conv<greyscalea>(double col) {
    return colour_conv<greyscalea>(colour_conv<uint8_t>(col));
}

template <>
inline rgba colour_conv<rgba>(double col) {
    return colour_conv<rgba>(colour_conv<uint8_t>(col));
}

} // namespace rol
