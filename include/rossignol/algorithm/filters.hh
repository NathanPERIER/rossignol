
#pragma once

#include "rossignol/image/image.hh"


namespace rol::algo {

void high_pass_unit_step_inplace(coefficient_plane& img, double threshold);
void low_pass_unit_step_inplace(coefficient_plane& img, double threshold);
void band_pass_unit_step_inplace(coefficient_plane& img, double low_threshold, double high_threshold);

} // namespace rol::algo
