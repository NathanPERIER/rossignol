
#pragma once

#include "rossignol/image/image.hh"
#include "rossignol/params/generic/greyscale_method.hh"


namespace rol::generic {

class to_greyscale {
public:
    to_greyscale(greyscale_method method = defaults::greyscale_method ): _method(method) {}

    greyscale_image operator()(const rgb_image& img) const;
    greyscale_image operator()(const greyscale_image& img) const { return img.share(); }
    greyscale_image operator()(const binary_image& img) const;
    greyscale_image operator()(const layer& img) const;
    greyscale_image operator()(const coefficient_plane& img) const;

    greyscale_image operator()(const image& img) const;

private:
    greyscale_method _method;
};

} // namespace rol::generic
