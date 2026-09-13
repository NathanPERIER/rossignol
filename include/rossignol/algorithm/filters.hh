
#pragma once

#include "rossignol/image/image.hh"
#include "rossignol/params/filters.hh"


namespace rol::algo {

/*--------------------+
|  Unit step          |
+--------------------*/

void unit_step_filter_inplace(coefficient_plane& img, const params::coefficient_filter& filter);

coefficient_plane unit_step_filter_mask(const coefficient_plane& img, const params::coefficient_filter& filter);


/*--------------------+
|  Sigmoid            |
+--------------------*/

void sigmoid_filter_inplace(coefficient_plane& img, const params::coefficient_filter& filter, double steepness = defaults::sigmoid_steepness);

coefficient_plane sigmoid_filter_mask(const coefficient_plane& img, const params::coefficient_filter& filter, double steepness = defaults::sigmoid_steepness);

} // namespace rol::algo
