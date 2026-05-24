#include "rossignol/generic/colourmap.hh"

#include "rossignol/algorithm/colourmap.hh"


namespace rol::generic {

rgb_image apply_colourmap::operator()(const rgb_image&) const {
    throw std::runtime_error("Unable to apply a colourmap on an RGB image (maybe extract a channel/coefficient or convert to greyscale ?)");
}

rgb_image apply_colourmap::operator()(const greyscale_image& img) const {
    return rol::algo::apply_colourmap(img, *_colourmap);
}

rgb_image apply_colourmap::operator()(const binary_image&) const {
    throw std::runtime_error("Unable to apply a colourmap on a binary image");
}

rgb_image apply_colourmap::operator()(const layer& img) const {
    return rol::algo::apply_colourmap(img, *_colourmap);
}

rgb_image apply_colourmap::operator()(const coefficient_plane& img) const {
    return rol::algo::apply_colourmap(img, *_colourmap);
}

rgb_image apply_colourmap::operator()(const image& img) const {
    return std::visit([this](const auto& img) -> rgb_image { return operator()(img); }, img);
}

} // namespace rol::generic
