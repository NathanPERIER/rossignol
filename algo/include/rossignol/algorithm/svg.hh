
#pragma once

#include "rossignol/image/image.hh"
#include "rossignol/params/svg_options.hh"


namespace rol::algo {

/// @brief Converts the image to an SVG where each pixel is drawn as a square
/// @note this is intended to be used with pixel art or dithered images
std::string to_svg(const rgb_image& img, const params::svg_options& opts = defaults::svg_options);

} // namespace rol::algo
