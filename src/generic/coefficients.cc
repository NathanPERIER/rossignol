#include "rossignol/generic/coefficients.hh"

#include "rossignol/algorithm/coefficients.hh"


namespace {

/// @brief extracts a coefficient from the image if possible, else throws an exception
template <rol::coefficient_type Coeff, typename Colour>
rol::coefficient_plane try_extract_coefficient(const rol::basic_image<Colour>& img) {
    if constexpr (rol::algo::detail::extractible_coefficient<Coeff, Colour>) {
        return rol::algo::extract_coefficient<Coeff>(img);
    } else {
        throw std::runtime_error("Unable to extract coefficient from the provided image type");
    }
}

template <typename Colour>
rol::coefficient_plane extract_coefficient_impl(const rol::basic_image<Colour>& img, const rol::coefficient_type& type) {
    switch (type) {
        case rol::coefficient_type::red:            return try_extract_coefficient<rol::coefficient_type::red>(img);
        case rol::coefficient_type::green:          return try_extract_coefficient<rol::coefficient_type::green>(img);
        case rol::coefficient_type::blue:           return try_extract_coefficient<rol::coefficient_type::blue>(img);
        case rol::coefficient_type::grey:           return try_extract_coefficient<rol::coefficient_type::grey>(img);
        case rol::coefficient_type::alpha:          return try_extract_coefficient<rol::coefficient_type::alpha>(img);
        case rol::coefficient_type::hue:            return try_extract_coefficient<rol::coefficient_type::hue>(img);
        case rol::coefficient_type::chroma:         return try_extract_coefficient<rol::coefficient_type::chroma>(img);
        case rol::coefficient_type::hsi_saturation: return try_extract_coefficient<rol::coefficient_type::hsi_saturation>(img);
        case rol::coefficient_type::hsl_saturation: return try_extract_coefficient<rol::coefficient_type::hsl_saturation>(img);
        case rol::coefficient_type::hsv_saturation: return try_extract_coefficient<rol::coefficient_type::hsv_saturation>(img);
        case rol::coefficient_type::lightness:      return try_extract_coefficient<rol::coefficient_type::lightness>(img);
        case rol::coefficient_type::value:          return try_extract_coefficient<rol::coefficient_type::value>(img);
        case rol::coefficient_type::intensity:      return try_extract_coefficient<rol::coefficient_type::intensity>(img);
        default: throw std::runtime_error("Unsupported coefficient type");
    }
}

} // anonymous namespace


namespace rol::generic {

coefficient_plane to_coefficients::operator()(const rgb_image&) const {
    throw std::runtime_error("Unable to convert an RGB image to coefficients (need an extracted layer)");
}

coefficient_plane to_coefficients::operator()(const greyscale_image&) const {
    throw std::runtime_error("Unable to convert a greyscale image to coefficients (need an extracted layer)");
}

coefficient_plane to_coefficients::operator()(const binary_image&) const {
    throw std::runtime_error("Unable to convert a binary image to coefficients (need an extracted layer)");
}

coefficient_plane to_coefficients::operator()(const layer& img) const {
    return rol::algo::to_coefficients(img);
}

coefficient_plane to_coefficients::operator()(const image& img) const {
    return std::visit([this](const auto& img) -> coefficient_plane { return operator()(img); }, img);
}


coefficient_plane extract_coefficient::operator()(const rgb_image& img) const {
    return ::extract_coefficient_impl(img, _type);
}

coefficient_plane extract_coefficient::operator()(const greyscale_image& img) const {
    return ::extract_coefficient_impl(img, _type);
}

coefficient_plane extract_coefficient::operator()(const layer&) const {
    throw std::runtime_error("Unable to extract coefficients from an unnamed channel (maybe use to_coefficients instead ?)");
}

coefficient_plane extract_coefficient::operator()(const coefficient_plane&) const {
    throw std::runtime_error("Unable to extract coefficients from an already extracted coefficient plane");
}

coefficient_plane extract_coefficient::operator()(const binary_image&) const {
    throw std::runtime_error("Unable to extract coefficients from a binary image");
}

coefficient_plane extract_coefficient::operator()(const image& img) const {
    return std::visit([this](const auto& img) -> coefficient_plane { return operator()(img); }, img);
}

} // namespace rol::generic
