#include "rossignol/generic/channels.hh"

#include "rossignol/algorithm/channels.hh"


namespace rol::generic {

layer extract_channel::operator()(const rgb_image& img) const {
    switch(_layer) {
        case rol::layer_name::red:   return rol::algo::extract_channel<rol::layer_name::red>(img);
        case rol::layer_name::green: return rol::algo::extract_channel<rol::layer_name::green>(img);
        case rol::layer_name::blue:  return rol::algo::extract_channel<rol::layer_name::blue>(img);
        case rol::layer_name::grey:  throw std::runtime_error("RGB image does not have a grey layer to extract");
        case rol::layer_name::alpha: return rol::algo::extract_channel<rol::layer_name::alpha>(img);
        default: throw std::runtime_error("Unsupported layer for extraction");
    }
}

layer extract_channel::operator()(const greyscale_image& img) const {
    switch(_layer) {
        case rol::layer_name::red:   // Implicitely fallback to grey
        case rol::layer_name::green: // Implicitely fallback to grey
        case rol::layer_name::blue:  // Implicitely fallback to grey
        case rol::layer_name::grey:  return rol::algo::extract_channel<rol::layer_name::grey>(img);
        case rol::layer_name::alpha: return rol::algo::extract_channel<rol::layer_name::alpha>(img);
        default: throw std::runtime_error("Unsupported layer for extraction");
    }
}

layer extract_channel::operator()(const binary_image& img) const {
    switch(_layer) {
        case rol::layer_name::red:   // Implicitely fallback to grey
        case rol::layer_name::green: // Implicitely fallback to grey
        case rol::layer_name::blue:  // Implicitely fallback to grey
        case rol::layer_name::grey:  return rol::algo::extract_channel(img); // Implicit conversion to grey
        case rol::layer_name::alpha: return rol::layer(img.size(), 255); // Implicitely fully opaque
        default: throw std::runtime_error("Unsupported layer for extraction");
    }
}

layer extract_channel::operator()(const layer&) const {
    throw std::runtime_error("Unable to extract layer from unnamed layer");
}

layer extract_channel::operator()(const coefficient_plane&) const {
    throw std::runtime_error("Unable to extract layer from coefficient plane (use direct conversion instead)");
}

layer extract_channel::operator()(const image& img) const {
    return std::visit([this](const auto& img) -> layer { return operator()(img); }, img);
}


rgb_image insert_channel::operator()(const rgb_image& img) const {
    const math::vec2i offset = compute_offset(_x_offset, _y_offset, _layer.size(), img.size());
    rgb_image res = img.clone();
    switch(_name) {
        case rol::layer_name::red:   rol::algo::insert_channel_inplace<rol::layer_name::red>(res, _layer, offset);   break;
        case rol::layer_name::green: rol::algo::insert_channel_inplace<rol::layer_name::green>(res, _layer, offset); break;
        case rol::layer_name::blue:  rol::algo::insert_channel_inplace<rol::layer_name::blue>(res, _layer, offset);  break;
        case rol::layer_name::grey:  throw std::runtime_error("RGB image does not have a grey layer for insertion");
        case rol::layer_name::alpha: rol::algo::insert_channel_inplace<rol::layer_name::alpha>(res, _layer, offset); break;
        default: throw std::runtime_error("Unsupported layer for insertion");
    }
    return res;
}

greyscale_image insert_channel::operator()(const greyscale_image& img) const {
    const math::vec2i offset = compute_offset(_x_offset, _y_offset, _layer.size(), img.size());
    greyscale_image res = img.clone();
    switch(_name) {
        case rol::layer_name::red:   throw std::runtime_error("Greyscale image does not have a red layer for insertion");
        case rol::layer_name::green: throw std::runtime_error("Greyscale image does not have a green layer for insertion");
        case rol::layer_name::blue:  throw std::runtime_error("Greyscale image does not have a blue layer for insertion");
        case rol::layer_name::grey:  rol::algo::insert_channel_inplace<rol::layer_name::grey>(res, _layer, offset);  break;
        case rol::layer_name::alpha: rol::algo::insert_channel_inplace<rol::layer_name::alpha>(res, _layer, offset); break;
        default: throw std::runtime_error("Unsupported layer for insertion");
    }
    return res;
}

binary_image insert_channel::operator()(const binary_image&) const {
    throw std::runtime_error("Unable to insert a channel in a binary image");
}

layer insert_channel::operator()(const layer&) const {
    throw std::runtime_error("Unable to insert a channel in an unnamed layer");
}

coefficient_plane insert_channel::operator()(const coefficient_plane&) const {
    throw std::runtime_error("Unable to insert a channel in a coefficient plane");
}

image insert_channel::operator()(const image& img) const {
    return std::visit([this](const auto& img) -> image { return operator()(img); }, img);
}


layer to_channel::operator()(const rgb_image&) const {
    throw std::runtime_error("Unable to convert an RGB image directly to an unnamed layer (use extraction instead)");
}

layer to_channel::operator()(const greyscale_image&) const {
    throw std::runtime_error("Unable to convert a greyscale image directly to an unnamed layer (use extraction instead)");
}

layer to_channel::operator()(const binary_image&) const {
    throw std::runtime_error("Unable to convert a binary image directly to an unnamed layer (use extraction instead)");
}

layer to_channel::operator()(const coefficient_plane& img) const {
    return rol::algo::to_channel(img);
}

layer to_channel::operator()(const image& img) const {
    return std::visit([this](const auto& img) -> layer { return operator()(img); }, img);
}

} // namespace rol::generic
