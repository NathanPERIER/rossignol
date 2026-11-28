
#pragma once

#include "rossignol/image/rgb_image.hh"
#include "rossignol/math/vec2.hh"
#include "rossignol/params/svg_options.hh"


namespace rol::algo {

/// @brief Area to be filled with a solid colour on an SVG image
struct svg_tile {
    using path = std::vector<math::vec2u>;

    rgba fill_colour;
    path outline;
    std::vector<path> holes = std::vector<path>();

    bool is_rectangle() const {
        if(!holes.empty() || outline.size() != 4) {
            return false;
        }
        if(outline[0].x == outline[1].x) {
            return (outline[1].y == outline[2].y)
                && (outline[2].x == outline[3].x)
                && (outline[3].y == outline[0].y);
        }
        if(outline[0].y == outline[1].y) {
            return (outline[1].x == outline[2].x)
                && (outline[2].y == outline[3].y)
                && (outline[3].x == outline[0].x);
        }
        return false;
    }

    bool operator==(const svg_tile&) const = default;
};

/// @brief Converts an image to coloured tiles
std::vector<svg_tile> get_svg_tiles(const rgb_image& img);


/// @brief Converts the image to an SVG where each pixel is drawn as a square
/// @note this is intended to be used with pixel art or dithered images
std::string to_svg(const rgb_image& img, const params::svg_options& opts = defaults::svg_options);

} // namespace rol::algo
