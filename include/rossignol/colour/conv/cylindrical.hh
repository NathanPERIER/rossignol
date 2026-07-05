
#pragma once

#include "rossignol/colour/hsi.hh"
#include "rossignol/colour/hsl.hh"
#include "rossignol/colour/hsv.hh"
#include "rossignol/colour/rgb.hh"
#include "rossignol/utils/typing/any_of.hh"


namespace rol {

double hue_coeff(const rgba& col);
double chroma(const rgba& col);

template <typename SaturationColour>
requires(any_of<SaturationColour, hsi, hsl, hsv>)
double saturation(const rgba& col);

uint8_t lightness(const rgba& col);
double lightness_coeff(const rgba& col);

inline uint8_t value(const rgba& col) { return std::max(std::max(col.r, col.g), col.b); }
inline double value_coeff(const rgba& col) { return static_cast<double>(value(col)) / 255.0; }

inline uint8_t intensity(const rgba& col) {
    return static_cast<uint8_t>(
        (static_cast<uint16_t>(col.r) + static_cast<uint16_t>(col.g) + static_cast<uint16_t>(col.b)) / 3
    );
};
inline double intensity_coeff(const rgba& col) {
    return (static_cast<double>(col.r) + static_cast<double>(col.g) + static_cast<double>(col.b)) / (3.0 * 255.0);
};


template <typename DestColour>
requires(any_of<DestColour, hsi, hsl, hsv>)
DestColour colour_conv(const rgba& col);

} // namespace rol
