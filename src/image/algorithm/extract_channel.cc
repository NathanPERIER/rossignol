#include "rossignol/image/algorithm/extract_channel.hh"

#include "rossignol/utils/typing/functional.hh"

#include "image/algorithm/pixel_mapper.hh"


namespace rol::detail {

/*--------------------+
|  RGBA               |
+--------------------*/

template<>
layer extract_channel<layer_name::red>(const rgb_image& img) {
    return rol::detail::map_pixels<uint8_t>(img, [](const rol::rgba& pixel) { return pixel.r; });
}

template<>
layer extract_channel<layer_name::green>(const rgb_image& img) {
    return rol::detail::map_pixels<uint8_t>(img, [](const rol::rgba& pixel) { return pixel.g; });
}

template<>
layer extract_channel<layer_name::blue>(const rgb_image& img) {
    return rol::detail::map_pixels<uint8_t>(img, [](const rol::rgba& pixel) { return pixel.b; });
}

template<>
layer extract_channel<layer_name::alpha>(const rgb_image& img) {
    return rol::detail::map_pixels<uint8_t>(img, [](const rol::rgba& pixel) { return pixel.a; });
}


/*--------------------+
|  GreyscaleA         |
+--------------------*/

template<>
layer extract_channel<layer_name::grey>(const greyscale_image& img) {
    return rol::detail::map_pixels<uint8_t>(img, [](const rol::greyscalea& pixel) { return pixel.grey; });
}

template<>
layer extract_channel<layer_name::alpha>(const greyscale_image& img) {
    return rol::detail::map_pixels<uint8_t>(img, [](const rol::greyscalea& pixel) { return pixel.a; });
}


/*--------------------+
|  Binary             |
+--------------------*/

layer extract_channel(const binary_image& img) {
    return rol::detail::map_pixels<uint8_t>(img, [](const bool pixel) { return static_cast<uint8_t>(pixel ? 255 : 0); });
}

} // namespace rol::detail


namespace {

class extract_channel_impl {
public:
    extract_channel_impl(rol::layer_name& layr): _layer(layr) {}

    rol::layer operator()(const rol::rgb_image& img) {
        switch(_layer) {
            case rol::layer_name::red:   return rol::detail::extract_channel<rol::layer_name::red>(img);
            case rol::layer_name::green: return rol::detail::extract_channel<rol::layer_name::green>(img);
            case rol::layer_name::blue:  return rol::detail::extract_channel<rol::layer_name::blue>(img);
            case rol::layer_name::grey:  throw std::runtime_error("RGB image does not have a grey layer to extract");
            case rol::layer_name::alpha: return rol::detail::extract_channel<rol::layer_name::alpha>(img);
            default: throw std::runtime_error("Unsupported layer for extraction");
        }
    }

    rol::layer operator()(const rol::greyscale_image& img) {
        switch(_layer) {
            case rol::layer_name::red:   // Implicitely fallback to grey
            case rol::layer_name::green: // Implicitely fallback to grey
            case rol::layer_name::blue:  // Implicitely fallback to grey
            case rol::layer_name::grey:  return rol::detail::extract_channel<rol::layer_name::grey>(img);
            case rol::layer_name::alpha: return rol::detail::extract_channel<rol::layer_name::alpha>(img);
            default: throw std::runtime_error("Unsupported layer for extraction");
        }
    }

    rol::layer operator()(const rol::binary_image& img) {
        switch(_layer) {
            case rol::layer_name::red:   // Implicitely fallback to grey
            case rol::layer_name::green: // Implicitely fallback to grey
            case rol::layer_name::blue:  // Implicitely fallback to grey
            case rol::layer_name::grey:  return rol::detail::extract_channel(img); // Implicit conversion to grey
            case rol::layer_name::alpha: return rol::layer(img.width(), img.height(), 255); // Implicitely fully opaque
            default: throw std::runtime_error("Unsupported layer for extraction");
        }
    }

    rol::layer operator()(const rol::layer&) {
        throw std::runtime_error("Unable to extract layer from unnamed layer");
    }

    rol::layer operator()(const rol::coefficient_plane&) {
        throw std::runtime_error("Unable to extract layer from coefficient plane");
    }

private:
    rol::layer_name _layer;
};

} // anonymous namespace


namespace rol {

layer extract_channel(const image& img, layer_name layr) {
    return std::visit(extract_channel_impl(layr), img);
}

} // namespace rol
