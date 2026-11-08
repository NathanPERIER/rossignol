#include "rossignol/algorithm/coefficients.hh"

#include "rossignol/algorithm/map_pixels.hh"
#include "rossignol/colour/conv/numeric.hh"


namespace rol::algo {

coefficient_plane to_coefficients(const layer& img) {
    return map_pixels<double>(img, [](const uint8_t& pixel) { return colour_conv<double>(pixel); });
}

} // namespace rol::algo
