#include "rossignol/generic/coefficients.hh"

#include "rossignol/algorithm/coefficients.hh"


namespace {

class to_coefficients_impl {
public:
    to_coefficients_impl(const rol::generic::to_coefficients& op): _op(op) {}

    rol::coefficient_plane operator()(const rol::rgb_image&) {
        throw std::runtime_error("Unable to convert an RGB image to coefficients (need an extracted layer)");
    }
    rol::coefficient_plane operator()(const rol::greyscale_image&) {
        throw std::runtime_error("Unable to convert a greyscale image to coefficients (need an extracted layer)");
    }
    rol::coefficient_plane operator()(const rol::binary_image&) {
        throw std::runtime_error("Unable to convert a binary image to coefficients (need an extracted layer)");
    }
    rol::coefficient_plane operator()(const rol::layer& img)             { return _op(img); }
    rol::coefficient_plane operator()(const rol::coefficient_plane& img) { return _op(img); }

private:
    const rol::generic::to_coefficients& _op;
};

} // anonymous namespace


namespace rol::generic {

coefficient_plane to_coefficients::operator()(const layer& img) const {
    return rol::algo::to_coefficients(img);
}

coefficient_plane to_coefficients::operator()(const image& img) const {
    return std::visit(::to_coefficients_impl(*this), img);
}

} // namespace rol::generic
