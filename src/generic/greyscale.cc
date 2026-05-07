#include "rossignol/generic/greyscale.hh"

#include "rossignol/algorithm/greyscale.hh"


namespace rol::generic {

greyscale_image to_greyscale::operator()(const rgb_image& img) const {
    switch(_method) {
        case rol::luma_method::sdtv: return rol::algo::greyscale<rol::luma_method::sdtv>(img);
        case rol::luma_method::hdtv: return rol::algo::greyscale<rol::luma_method::hdtv>(img);
        case rol::luma_method::hdr:  return rol::algo::greyscale<rol::luma_method::hdr>(img);
        default: throw std::runtime_error("Unsupported luma method");
    }
}

greyscale_image to_greyscale::operator()(const binary_image& img) const {
    return rol::algo::greyscale(img);
}

greyscale_image to_greyscale::operator()(const layer& img) const {
    return rol::algo::greyscale(img);
}

greyscale_image to_greyscale::operator()(const coefficient_plane& img) const {
    return rol::algo::greyscale(img);
}

greyscale_image to_greyscale::operator()(const image& img) const {
    return std::visit(*this, img);
}

} // namespace rol::generic
