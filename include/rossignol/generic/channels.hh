
#pragma once

#include "rossignol/image/image.hh"
#include "rossignol/params/layer_name.hh"
#include "rossignol/params/generic/offset.hh"


namespace rol::generic {

class extract_channel {
public:
    extract_channel(layer_name layr): _layer(layr) {}

    layer operator()(const rgb_image& img) const;
    layer operator()(const greyscale_image& img) const;
    layer operator()(const binary_image& img) const;

    layer operator()(const image& img) const;

private:
    layer_name _layer;

    layer operator()(const layer&) const;
    layer operator()(const coefficient_plane&) const;
};


class insert_channel {
public:
    insert_channel(layer layr, layer_name name, horizontal_offset x_offset, vertical_offset y_offset):
        _layer(std::move(layr)), _name(name), _x_offset(x_offset), _y_offset(y_offset) {}
    insert_channel(layer layr, layer_name name):
        insert_channel(std::move(layr), name, defaults::horizontal_offset, defaults::vertical_offset) {}

    rgb_image operator()(const rgb_image& img) const;
    greyscale_image operator()(const greyscale_image& img) const;

    image operator()(const image& img) const;

    private:
    layer _layer;
    layer_name _name;
    horizontal_offset _x_offset;
    vertical_offset _y_offset;

    binary_image operator()(const binary_image&) const;
    layer operator()(const layer&) const;
    coefficient_plane operator()(const coefficient_plane&) const;
};


class to_channel {
public:
    to_channel() = default;

    layer operator()(const layer& img) const { return img.share(); }
    layer operator()(const coefficient_plane& img) const;

    layer operator()(const image& img) const;

private:
    layer operator()(const rgb_image&) const;
    layer operator()(const greyscale_image&) const;
    layer operator()(const binary_image&) const;
};

} // namespace rol::generic
