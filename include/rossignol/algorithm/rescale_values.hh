
#pragma once

#include "rossignol/image/image.hh"
#include "rossignol/params/limits.hh"


namespace rol::algo {

void rescale_values_inplace(layer& img, uint8_t min = defaults::channel_limits<uint8_t>::min, uint8_t max = defaults::channel_limits<uint8_t>::max);

void rescale_values_inplace(coefficient_plane& img, double min = defaults::channel_limits<double>::min, double max = defaults::channel_limits<double>::max);

} // namespace rol::algo
