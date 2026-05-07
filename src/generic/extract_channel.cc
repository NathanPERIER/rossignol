#include "rossignol/generic/extract_channel.hh"

#include "rossignol/algorithm/extract_channel.hh"



namespace {

class extract_channel_impl {
public:
    extract_channel_impl(const rol::generic::extract_channel& op): _op(op) {}

    rol::layer operator()(const rol::rgb_image& img)       { return _op(img); }
    rol::layer operator()(const rol::greyscale_image& img) { return _op(img); }
    rol::layer operator()(const rol::binary_image& img)    { return _op(img); }
    rol::layer operator()(const rol::layer&) {
        throw std::runtime_error("Unable to extract layer from unnamed layer");
    }
    rol::layer operator()(const rol::coefficient_plane&) {
        throw std::runtime_error("Unable to extract layer from coefficient plane (use direct conversion instead)");
    }

private:
    const rol::generic::extract_channel& _op;
};


struct to_channel_impl {
public:
    to_channel_impl(const rol::generic::to_channel& op): _op(op) {}

    rol::layer operator()(const rol::rgb_image&) {
        throw std::runtime_error("Unable to convert an RGB image directly to an unnamed layer (use extraction instead)");
    }
    rol::layer operator()(const rol::greyscale_image&) {
        throw std::runtime_error("Unable to convert a greyscale image directly to an unnamed layer (use extraction instead)");
    }
    rol::layer operator()(const rol::binary_image&) {
        throw std::runtime_error("Unable to convert a binary image directly to an unnamed layer (use extraction instead)");
    }
    rol::layer operator()(const rol::layer& img)             { return _op(img); }
    rol::layer operator()(const rol::coefficient_plane& img) { return _op(img); }

private:
    const rol::generic::to_channel& _op;
};

} // anonymous namespace


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

layer extract_channel::operator()(const image& img) const {
    return std::visit(::extract_channel_impl(*this), img);
}


layer to_channel::operator()(const coefficient_plane& img) const {
    return rol::algo::to_channel(img);
}

layer to_channel::operator()(const image& img) const {
    return std::visit(::to_channel_impl(*this), img);
}

} // namespace rol::generic
