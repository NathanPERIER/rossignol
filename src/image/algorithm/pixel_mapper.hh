
#pragma once

#include "rossignol/image/image.hh"


namespace rol::detail {

template <typename ColourOut, typename ColourIn, typename ColourMapper>
// requires(...)
basic_image<ColourOut> map_pixels(const basic_image<ColourIn>& img, const ColourMapper& transform) {
    basic_image<ColourOut> res(img.width(), img.height());

    for(std::size_t y = 0; y < img.height(); y++) {
        std::span<const ColourIn> in_row = img[y];
        std::span<ColourOut> out_row = res[y];
        for(std::size_t x = 0; x < img.width(); x++) {
            out_row[x] = transform(in_row[x]);
        }
    }

    return res;
}

} // namespace rol::detail
