#include "rossignol/algorithm/coefficients.hh"

#include "rossignol/algorithm/map_pixels.hh"


namespace rol::algo {

coefficient_plane to_coefficients(const layer& img) {
    return map_pixels<double>(img, [](const uint8_t& pixel) { return static_cast<double>(pixel) / 255.0; });
}

} // namespace rol::algo
