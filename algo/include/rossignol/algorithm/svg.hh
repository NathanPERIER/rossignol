
#pragma once

#include "rossignol/image/image.hh"
#include "rossignol/params/svg_options.hh"


namespace rol::algo {

std::string to_svg(const rgb_image& img, const params::svg_options& opts = defaults::svg_options);

} // namespace rol::algo
