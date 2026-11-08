#include "rossignol/generic/rgb.hh"

#include "rossignol/algorithm/rgb.hh"


namespace rol::generic {

rgb_image to_rgb::operator()(const greyscale_image& img) const {
    return rol::algo::to_rgb(img);
}

rgb_image to_rgb::operator()(const binary_image& img) const {
    return rol::algo::to_rgb(img);
}

rgb_image to_rgb::operator()(const layer& img) const {
    return rol::algo::to_rgb(img);
}

rgb_image to_rgb::operator()(const coefficient_plane& img) const {
    return rol::algo::to_rgb(img);
}

rgb_image to_rgb::operator()(const image& img) const {
    return std::visit(*this, img);
}

} // namespace rol::generic
