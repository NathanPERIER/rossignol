
#pragma once

#include "rossignol/image/image.hh"
#include "rossignol/params/colourise.hh"
#include "rossignol/params/generic/cylindrical_colour_type.hh"


namespace rol::generic {

class colourise {
public:
    colourise(cylindrical_colour_type cyl_type = defaults::cylindrical_colour_type, double hue = rol::defaults::colourise_hue, double saturation = rol::defaults::colourise_saturation):
        _cyl_type(cyl_type), _hue(hue), _saturation(saturation) {}

    rgb_image operator()(const layer& img) const;
    rgb_image operator()(const coefficient_plane& img) const;
    rgb_image operator()(const greyscale_image& img) const;

    rgb_image operator()(const image& img) const;

private:
    cylindrical_colour_type _cyl_type;
    double _hue;
    double _saturation;

    rgb_image operator()(const binary_image&) const;
    rgb_image operator()(const rgb_image&) const;
};

} // namespace rol::generic
