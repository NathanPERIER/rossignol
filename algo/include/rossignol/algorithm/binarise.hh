
#pragma once

#include "rossignol/image/image.hh"
#include "rossignol/params/binarise.hh"


namespace rol::algo {

binary_image floyd_steinberg(const greyscale_image& img);

binary_image binarise_threshold(const greyscale_image& img, uint8_t threshold = defaults::binary_threshold);

} // namespace rol::algo
