
#pragma once

#include "rossignol/image/image.hh"


namespace rol::detail {

template <typename Colour>
basic_image<Colour> vertical_mirror(const basic_image<Colour>& img) {
    basic_image<Colour> res = img.share();
    res.invert_y();
    return res;
}

template <typename Colour>
basic_image<Colour> horizontal_mirror(const basic_image<Colour>& img) {
    basic_image<Colour> res(img.width(), img.height());
    for(std::size_t y = 0; y < img.height(); y++) {
        std::copy(img[y].rbegin(), img[y].rend(), res[y].begin());
    }
    return res;
}

} // namespace rol::detail


namespace rol {

enum class mirror_direction {
    horizontal,
    vertical
};

image mirror(const image& img, mirror_direction direction);

} // namespace rol
