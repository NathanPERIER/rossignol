
#pragma once

#include "rossignol/colour/generic.hh"
#include "rossignol/image/image.hh"
#include "rossignol/params/generic/offset.hh"
#include "rossignol/params/generic/size.hh"


namespace rol::generic {

class fill {
public:
    fill(colour col, image_length width, image_length height, horizontal_offset x_offset, vertical_offset y_offset):
        _colour(col), _width(width), _height(height), _x_offset(x_offset), _y_offset(y_offset) {}
    fill(colour col, image_length width, image_length height):
        fill(col, width, height, defaults::horizontal_offset, defaults::vertical_offset) {}
    fill(colour col): fill(col, percentage{ .value = 1.0 }, percentage{ .value = 1.0 }) {}

    rgb_image operator()(const rgb_image& img) const;
    greyscale_image operator()(const greyscale_image& img) const;
    binary_image operator()(const binary_image& img) const;
    layer operator()(const layer& img) const;
    coefficient_plane operator()(const coefficient_plane& img) const;

    image operator()(const image& img) const;

private:
    colour _colour;
    image_length _width;
    image_length _height;
    horizontal_offset _x_offset;
    vertical_offset _y_offset;

    template <typename Colour>
    basic_image<Colour> fill_impl(const basic_image<Colour>& img) const;
};

} // namespace rol::generic
