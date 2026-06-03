#pragma once

#include "rossignol/image/image.hh"
#include "rossignol/math/vec2.hh"
#include "rossignol/params/binary_diff_palette.hh"


namespace rol::algo {

/// @brief computes a layer where each pixel is the absolute difference between the corresponding pixels of the two input layers
layer distance(const layer& l1, const layer& l2, math::vec2i offset);

/// @brief computes a coefficient plane where each pixel is the absolute difference between the corresponding coefficients of the two input layers
coefficient_plane distance(const coefficient_plane& p1, const coefficient_plane& p2, math::vec2i offset);

/// @brief computes a coefficient plane where each pixel is the difference between the corresponding coefficients of the two input layers, scaled back to the `[0, 1]` range
coefficient_plane diff(const coefficient_plane& original, const coefficient_plane& updated, math::vec2i offset);

/// @brief makes an image showing pixels that have changed between both bit planes
rgb_image diff(const binary_image& original, const binary_image& updated, math::vec2i offset, const params::binary_diff_palette& palette = defaults::binary_diff_palette);

} // namespace rol::algo
