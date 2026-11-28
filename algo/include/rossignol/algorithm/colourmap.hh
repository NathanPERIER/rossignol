
#pragma once

#include "rossignol/image/channel.hh"
#include "rossignol/image/coefficient_plane.hh"
#include "rossignol/image/greyscale_image.hh"
#include "rossignol/image/rgb_image.hh"
#include "rossignol/colour/colourmap.hh"


namespace rol::algo {

rgb_image apply_colourmap(const rol::greyscale_image& img, const rol::colourmap& cmap);

rgb_image apply_colourmap(const rol::layer& img, const rol::colourmap& cmap);

rgb_image apply_colourmap(const rol::coefficient_plane& img, const rol::colourmap& cmap);

} // namespace rol::algo
