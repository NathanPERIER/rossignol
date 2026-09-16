#include "rossignol/generic/rescale_values.hh"

#include "rossignol/algorithm/rescale_values.hh"


namespace rol::generic {

template <rol::any_of<uint8_t, double> Colour>
rol::basic_image<Colour> rescale_values::operator()(const rol::basic_image<Colour>& img) const {
    rol::basic_image<Colour> res = img.clone();
    rol::algo::rescale_values_inplace(res, _min.as<Colour>(), _max.as<Colour>());
    return res;
}

image rescale_values::operator()(const rgb_image&) const {
    throw std::runtime_error("Cannot rescale values in an RGB image");
}

image rescale_values::operator()(const greyscale_image&) const {
    throw std::runtime_error("Cannot rescale values in a greyscale image");
}

image rescale_values::operator()(const binary_image&) const {
    throw std::runtime_error("Cannot rescale values in a binary image");
}

image rescale_values::operator()(const image& img) const {
    return std::visit([this](const auto& img) -> image { return operator()(img); }, img);
}

} // namespace rol::generic
