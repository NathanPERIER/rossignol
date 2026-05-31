#include "rossignol/algorithm/channels.hh"

#include "rossignol/colour/conv/numeric.hh"


namespace rol::algo {


/*--------------------+
|  Binary             |
+--------------------*/

layer extract_channel(const binary_image& img) {
    return map_pixels<uint8_t>(img, [](const bool pixel) { return colour_conv<uint8_t>(pixel); });
}


/*--------------------+
|  Coefficients       |
+--------------------*/

layer to_channel(const coefficient_plane& img) {
    return map_pixels<uint8_t>(img, [](const double& pixel) { return colour_conv<uint8_t>(pixel); });
}

} // namespace rol::algo
