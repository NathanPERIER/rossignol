#include "rossignol/generic/invert.hh"

#include "rossignol/algorithm/invert.hh"


namespace rol::generic {

rgb_image invert::operator()(const rgb_image& img) const {
    return rol::algo::invert(img);
}

greyscale_image invert::operator()(const greyscale_image& img) const {
    return rol::algo::invert(img);
}

binary_image invert::operator()(const binary_image& img) const {
    return rol::algo::invert(img);
}

layer invert::operator()(const layer& img) const {
    return rol::algo::invert(img);
}

coefficient_plane invert::operator()(const coefficient_plane& img) const {
    return rol::algo::invert(img);
}

image invert::operator()(const image& img) const {
    return std::visit([this](const auto& img) -> image { return operator()(img); }, img);
}

} // namespace rol::generic
