#include "rossignol/algorithm/colourmap.hh"

#include "rossignol/algorithm/map_pixels.hh"


namespace rol::algo {

rgb_image apply_colourmap(const rol::greyscale_image& img, const rol::colourmap& cmap) {
    return map_pixels<rol::rgba>(img, [&cmap](const rol::greyscalea& pixel) {
        rol::rgba res = cmap[pixel.grey];
        res.a = pixel.a;
        return res;
    });
}

rgb_image apply_colourmap(const rol::layer& img, const rol::colourmap& cmap) {
    return map_pixels<rol::rgba>(img, [&cmap](const uint8_t& pixel) { return cmap[pixel]; });
}

rgb_image apply_colourmap(const rol::coefficient_plane& img, const rol::colourmap& cmap) {
    return map_pixels<rol::rgba>(img, cmap);
}

} // namespace rol::algo
