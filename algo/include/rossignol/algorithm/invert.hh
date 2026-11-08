
#pragma once

#include "rossignol/algorithm/map_pixels.hh"
#include "rossignol/colour/conv/invert.hh"
#include "rossignol/image/image.hh"


namespace rol::algo {

template <typename Colour>
void invert_inplace(basic_image<Colour>& img) {
    for(std::size_t y = 0; y < img.height(); y++) {
        std::span<Colour> row = img[y];
        for(std::size_t x = 0; x < img.width(); x++) {
            invert_inplace(row[x]);
        }
    }
}

template <typename Colour>
basic_image<Colour> invert(const basic_image<Colour>& img) {
    return map_pixels<Colour>(img, [](const Colour& col) { return rol::invert(col); });
}

} // namespace rol::algo
