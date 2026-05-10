
#pragma once

#include "rossignol/image/image.hh"
#include "rossignol/utils/typing/functional.hh"


namespace rol::algo {

template <typename ColourOut, typename ColourIn, typename ColourMapper>
requires(yield_invocable<ColourMapper, ColourOut, ColourIn>)
basic_image<ColourOut> map_pixels(const basic_image<ColourIn>& img, const ColourMapper& transform) {
    basic_image<ColourOut> res(img.size());

    for(std::size_t y = 0; y < img.height(); y++) {
        std::span<const ColourIn> in_row = img[y];
        std::span<ColourOut> out_row = res[y];
        for(std::size_t x = 0; x < img.width(); x++) {
            out_row[x] = transform(in_row[x]);
        }
    }

    return res;
}

} // namespace rol::algo
