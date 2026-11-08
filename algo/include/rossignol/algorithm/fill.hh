
#pragma once

#include "rossignol/image/image.hh"
#include "rossignol/math/rect.hh"
#include "rossignol/params/alignment.hh"


namespace rol::algo {

template <typename Colour>
bool fill_inplace(basic_image<Colour>& img, Colour col, const math::rect& area) {
    const math::rect intersection = area.intersect(math::rect(img.size()));
    if(intersection.empty()) {
        return false;
    }
    for(std::size_t y = static_cast<std::size_t>(intersection.y1()); y < static_cast<std::size_t>(intersection.y2()); y++) {
        std::span<Colour> row = img[y];
        std::fill(row.begin() + static_cast<std::size_t>(intersection.x1()), row.begin() + static_cast<std::size_t>(intersection.x2()), col);
    }
    return true;
}

template <typename Colour>
bool fill_inplace(basic_image<Colour>& img, Colour col, const math::vec2u& size, const math::vec2i& offset) {
    return fill_inplace(img, col, math::rect(size) + offset);
}

template <typename Colour>
bool fill_inplace(basic_image<Colour>& img, Colour col, const math::vec2u& size) {
    return fill_inplace(img, col, size, math::vec2i{ .x = 0, .y = 0 });
}

template <typename Colour>
bool fill_inplace(basic_image<Colour>& img, Colour col) {
    return fill_inplace(img, col, img.size());
}

} // namespace rol::algo