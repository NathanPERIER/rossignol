
#pragma once

#include "rossignol/image/image.hh"
#include "rossignol/params/filters.hh"


namespace rol::algo {

void high_pass_unit_step_inplace(coefficient_plane& img, double threshold);
void low_pass_unit_step_inplace(coefficient_plane& img, double threshold);
void band_pass_unit_step_inplace(coefficient_plane& img, double low_threshold, double high_threshold);

void high_pass_sigmoid_inplace(coefficient_plane& img, double threshold, double steepness = defaults::sigmoid_steepness);
void low_pass_sigmoid_inplace(coefficient_plane& img, double threshold, double steepness = defaults::sigmoid_steepness);
void band_pass_sigmoid_inplace(coefficient_plane& img, double low_threshold, double high_threshold, double steepness = defaults::sigmoid_steepness);

} // namespace rol::algo
