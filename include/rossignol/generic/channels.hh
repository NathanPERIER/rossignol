
#pragma once

#include "rossignol/image/image.hh"
#include "rossignol/params/layer_name.hh"


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
