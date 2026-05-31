#include "rossignol/algorithm/channels.hh"

#include "rossignol/algorithm/map_pixels.hh"
#include "rossignol/colour/conv/numeric.hh"


namespace rol::algo {

/*--------------------+
|  RGBA               |
+--------------------*/

template<>
layer extract_channel<layer_name::red>(const rgb_image& img) {
    return map_pixels<uint8_t>(img, [](const rol::rgba& pixel) { return pixel.r; });
}

template<>
layer extract_channel<layer_name::green>(const rgb_image& img) {
    return map_pixels<uint8_t>(img, [](const rol::rgba& pixel) { return pixel.g; });
}

template<>
layer extract_channel<layer_name::blue>(const rgb_image& img) {
    return map_pixels<uint8_t>(img, [](const rol::rgba& pixel) { return pixel.b; });
}

template<>
layer extract_channel<layer_name::alpha>(const rgb_image& img) {
    return map_pixels<uint8_t>(img, [](const rol::rgba& pixel) { return pixel.a; });
}


/*--------------------+
|  GreyscaleA         |
+--------------------*/

template<>
layer extract_channel<layer_name::grey>(const greyscale_image& img) {
    return map_pixels<uint8_t>(img, [](const rol::greyscalea& pixel) { return pixel.grey; });
}

template<>
layer extract_channel<layer_name::alpha>(const greyscale_image& img) {
    return map_pixels<uint8_t>(img, [](const rol::greyscalea& pixel) { return pixel.a; });
}


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
