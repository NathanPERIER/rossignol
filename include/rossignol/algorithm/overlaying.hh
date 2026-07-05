
#pragma once

#include "rossignol/image/image.hh"
#include "rossignol/math/rect.hh"
#include "rossignol/utils/typing/functional.hh"


namespace rol::algo {

/// @brief helper to work on the intersection of two overlayed images
/// @tparam ColourBg colour of the background image
/// @tparam ColourFg colour of the foreground image
/// @param background background image
/// @param foreground foreground image
/// @param offset offset of the foreground image relative to the background image
/// @param cb method to apply on the intersection
/// @return true if the intersecion is not empty (and the callback was applied)
template <typename ColourBg, typename ColourFg, typename Callback>
requires(yield_invocable<Callback, void, const math::vec2u&, basic_image<ColourBg>&, const math::vec2u&, const basic_image<ColourFg>&, const math::vec2u&>)
bool overlayed_images_intersection(basic_image<ColourBg>& background, const basic_image<ColourFg>& foreground, const math::vec2i& offset, const Callback& cb) {
    // This is the intersection area in the foreground's frame of reference
    const math::rect intersection = math::rect(foreground.size()).intersect(math::rect(background.size()) - offset);
    if(intersection.empty()) {
        return false;
    }
    // Size of the common area
    const math::vec2u area_size = intersection.size();
    // Beginning of the area in the background image's frame of reference
    const math::vec2u bg_start {
        .x = (offset.x >= 0) ? static_cast<std::size_t>(offset.x) : 0,
        .y = (offset.y >= 0) ? static_cast<std::size_t>(offset.y) : 0
    };
    // Beginning of the area in the foreground image's frame of reference
    const math::vec2u fg_start {
        .x = static_cast<std::size_t>(intersection.x1()),
        .y = static_cast<std::size_t>(intersection.y1())
    };
    // Invoke the callback
    cb(area_size, background, bg_start, foreground, fg_start);
    return true;
}

} // namespace rol::algo
