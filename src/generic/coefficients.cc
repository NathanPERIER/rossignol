#include "rossignol/generic/coefficients.hh"

#include "rossignol/algorithm/coefficients.hh"


namespace rol::generic {

coefficient_plane to_coefficients::operator()(const rgb_image&) const {
    throw std::runtime_error("Unable to convert an RGB image to coefficients (need an extracted layer)");
}

coefficient_plane to_coefficients::operator()(const greyscale_image&) const {
    throw std::runtime_error("Unable to convert a greyscale image to coefficients (need an extracted layer)");
}

coefficient_plane to_coefficients::operator()(const binary_image&) const {
    throw std::runtime_error("Unable to convert a binary image to coefficients (need an extracted layer)");
}

coefficient_plane to_coefficients::operator()(const layer& img) const {
    return rol::algo::to_coefficients(img);
}

coefficient_plane to_coefficients::operator()(const image& img) const {
    return std::visit([this](const auto& img) -> coefficient_plane { return operator()(img); }, img);
}

} // namespace rol::generic
