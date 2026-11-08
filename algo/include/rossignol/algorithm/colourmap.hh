
#pragma once

#include "rossignol/image/image.hh"
#include "rossignol/colour/colourmap.hh"


namespace rol::algo {

rgb_image apply_colourmap(const rol::greyscale_image& img, const rol::colourmap& cmap);

rgb_image apply_colourmap(const rol::layer& img, const rol::colourmap& cmap);

rgb_image apply_colourmap(const rol::coefficient_plane& img, const rol::colourmap& cmap);

} // namespace rol::algo
