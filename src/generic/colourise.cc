#include "rossignol/generic/colourise.hh"

#include "rossignol/algorithm/colourise.hh"

namespace {

template <typename Colour>
rol::rgb_image colourise_impl(const rol::basic_image<Colour>& img, rol::generic::cylindrical_colour_type cyl_type, const double& hue, const double& saturation) {
    switch(cyl_type) {
        case rol::generic::cylindrical_colour_type::hsi: return rol::algo::colourise<rol::hsi>(img, hue, saturation);
        case rol::generic::cylindrical_colour_type::hsl: return rol::algo::colourise<rol::hsl>(img, hue, saturation);
        case rol::generic::cylindrical_colour_type::hsv: return rol::algo::colourise<rol::hsv>(img, hue, saturation);
    }
    throw std::runtime_error("Got invalid cylindrical colour type");
}

} // anonymous namespace


namespace rol::generic {

rgb_image colourise::operator()(const layer& img) const {
    return ::colourise_impl(img, _cyl_type, _hue, _saturation);
}

rgb_image colourise::operator()(const coefficient_plane& img) const {
    return ::colourise_impl(img, _cyl_type, _hue, _saturation);
}

rgb_image colourise::operator()(const greyscale_image& img) const {
    return ::colourise_impl(img, _cyl_type, _hue, _saturation);
}

} // namespace rol::generic
