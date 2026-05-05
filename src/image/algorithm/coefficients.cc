#include "rossignol/image/algorithm/coefficients.hh"

#include "image/algorithm/pixel_mapper.hh"


namespace rol::detail {

coefficient_plane to_coefficients(const layer& img) {
    return rol::detail::map_pixels<double>(img, [](const uint8_t& pixel) { return static_cast<double>(pixel) / 255.0; });
}

} // namespace rol::detail


namespace {

struct to_coefficients_impl {
    rol::coefficient_plane operator()(const rol::rgb_image&) {
        throw std::runtime_error("Unable to convert an RGB image to coefficients (need an extracted layer)");
    }
    rol::coefficient_plane operator()(const rol::greyscale_image&) {
        throw std::runtime_error("Unable to convert a greyscale image to coefficients (need an extracted layer)");
    }
    rol::coefficient_plane operator()(const rol::binary_image&) {
        throw std::runtime_error("Unable to convert a binary image to coefficients (need an extracted layer)");
    }
    rol::coefficient_plane operator()(const rol::layer& img) {
        return rol::detail::to_coefficients(img);
    }
    rol::coefficient_plane operator()(const rol::coefficient_plane& img) {
        return img.share();
    }
};


} // anonymous namespace


namespace rol {

coefficient_plane to_coefficients(const image& img) {
    return std::visit(to_coefficients_impl{}, img);
}

} // namespace rol
