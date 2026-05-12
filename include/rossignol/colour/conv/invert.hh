
#pragma once

#include "rossignol/colour/greyscale.hh"
#include "rossignol/colour/rgb.hh"


namespace rol {

template <typename Colour>
void invert_inplace(Colour& pixel);

template <>
void invert_inplace<rgba>(rgba& pixel) {
	pixel.r = ~pixel.r;
	pixel.g = ~pixel.g;
	pixel.b = ~pixel.b;
}

template <>
void invert_inplace<greyscalea>(greyscalea& pixel) {
	pixel.grey = ~pixel.grey;
}

template <>
void invert_inplace<bool>(bool& pixel) {
	pixel = !pixel;
}

template <>
void invert_inplace<uint8_t>(uint8_t& pixel) {
	pixel = ~pixel;
}

template <>
void invert_inplace<double>(double& pixel) {
	pixel = 1.0 - pixel;
}


template <typename Colour>
Colour invert(const Colour& pixel) {
    Colour res = pixel;
    invert_inplace(res);
    return res;
}

} // namespace rol
