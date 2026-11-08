#include "rossignol/generic/bit_planes.hh"

#include "rossignol/algorithm/bit_planes.hh"


namespace rol::generic {

binary_image extract_bit_plane::operator()(const rgb_image&) const {
    throw std::runtime_error("Cannot extract a bit plane from an RGB image (extract a channel first)");
}

binary_image extract_bit_plane::operator()(const greyscale_image&) const {
    throw std::runtime_error("Cannot extract a bit plane from a greyscale image (extract a channel first)");
}

binary_image extract_bit_plane::operator()(const binary_image&) const {
    throw std::runtime_error("Cannot extract a bit plane from a binary image");
}

binary_image extract_bit_plane::operator()(const coefficient_plane&) const {
    throw std::runtime_error("Cannot extract a bit plane from a coefficient plane");
}

binary_image extract_bit_plane::operator()(const layer& img) const {
    switch(_index) {
        case 0: return algo::extract_bit_plane<0>(img);
        case 1: return algo::extract_bit_plane<1>(img);
        case 2: return algo::extract_bit_plane<2>(img);
        case 3: return algo::extract_bit_plane<3>(img);
        case 4: return algo::extract_bit_plane<4>(img);
        case 5: return algo::extract_bit_plane<5>(img);
        case 6: return algo::extract_bit_plane<6>(img);
        case 7: return algo::extract_bit_plane<7>(img);
    }
    throw std::runtime_error("Bad index for bit plane extraction");
}

binary_image extract_bit_plane::operator()(const image& img) const {
    return std::visit([*this](const auto& img) { return operator()(img); }, img);
}

} // namespace rol::generic