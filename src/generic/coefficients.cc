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

/// @brief inserts a coefficient in the image if possible, else throws an exception
template <rol::coefficient_type Coeff, typename Colour>
rol::basic_image<Colour> try_insert_coefficient(const rol::basic_image<Colour>& img, const rol::coefficient_plane& coeffs, const rol::math::vec2i& offset) {
    if constexpr (rol::algo::detail::editable_coefficient<Coeff, Colour>) {
        rol::basic_image<Colour> res = img.clone();
        rol::algo::insert_coefficient_inplace<Coeff>(res, coeffs, offset);
        return res;
    } else {
        throw std::runtime_error("Unable to insert coefficient in the provided image type");
    }
}

template <typename Colour>
rol::basic_image<Colour> insert_coefficient_impl(const rol::basic_image<Colour>& img, const rol::coefficient_type& type, const rol::coefficient_plane& coeffs, const rol::math::vec2i& offset) {
    switch (type) {
        case rol::coefficient_type::red:            return try_insert_coefficient<rol::coefficient_type::red>(img, coeffs, offset);
        case rol::coefficient_type::green:          return try_insert_coefficient<rol::coefficient_type::green>(img, coeffs, offset);
        case rol::coefficient_type::blue:           return try_insert_coefficient<rol::coefficient_type::blue>(img, coeffs, offset);
        case rol::coefficient_type::grey:           return try_insert_coefficient<rol::coefficient_type::grey>(img, coeffs, offset);
        case rol::coefficient_type::alpha:          return try_insert_coefficient<rol::coefficient_type::alpha>(img, coeffs, offset);
        case rol::coefficient_type::hue:            return try_insert_coefficient<rol::coefficient_type::hue>(img, coeffs, offset);
        case rol::coefficient_type::chroma:         return try_insert_coefficient<rol::coefficient_type::chroma>(img, coeffs, offset);
        case rol::coefficient_type::hsi_saturation: return try_insert_coefficient<rol::coefficient_type::hsi_saturation>(img, coeffs, offset);
        case rol::coefficient_type::hsl_saturation: return try_insert_coefficient<rol::coefficient_type::hsl_saturation>(img, coeffs, offset);
        case rol::coefficient_type::hsv_saturation: return try_insert_coefficient<rol::coefficient_type::hsv_saturation>(img, coeffs, offset);
        case rol::coefficient_type::lightness:      return try_insert_coefficient<rol::coefficient_type::lightness>(img, coeffs, offset);
        case rol::coefficient_type::value:          return try_insert_coefficient<rol::coefficient_type::value>(img, coeffs, offset);
        case rol::coefficient_type::intensity:      return try_insert_coefficient<rol::coefficient_type::intensity>(img, coeffs, offset);
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


math::vec2i insert_coefficient::get_offset(const rol::math::vec2u& input_size) const {
    return compute_offset(_x_offset, _y_offset, _coeffs.size(), input_size);
}

rgb_image insert_coefficient::operator()(const rgb_image& img) const {
    return ::insert_coefficient_impl(img, _type, _coeffs, get_offset(img.size()));
}

greyscale_image insert_coefficient::operator()(const greyscale_image& img) const {
    return ::insert_coefficient_impl(img, _type, _coeffs, get_offset(img.size()));
}

binary_image insert_coefficient::operator()(const binary_image&) const {
    throw std::runtime_error("Unable to insert coefficients in a binary image");
}

layer insert_coefficient::operator()(const layer&) const {
    throw std::runtime_error("Unable to insert coefficients in an unnamed channel (maybe use to_channel instead ?)");

}
coefficient_plane insert_coefficient::operator()(const coefficient_plane&) const {
    throw std::runtime_error("Unable to insert coefficients in a coefficient plane");
}

image insert_coefficient::operator()(const image& img) const {
    return std::visit([this](const auto& img) -> image { return operator()(img); }, img);
}

} // namespace rol::generic
