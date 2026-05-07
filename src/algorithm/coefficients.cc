#include "rossignol/algorithm/coefficients.hh"

#include "image/algorithm/pixel_mapper.hh"


namespace rol::detail {

coefficient_plane to_coefficients(const layer& img) {
    return rol::detail::map_pixels<double>(img, [](const uint8_t& pixel) { return static_cast<double>(pixel) / 255.0; });
}

} // namespace rol::detail
