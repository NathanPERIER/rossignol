#include "rossignol/generic/fill.hh"

#include "rossignol/algorithm/fill.hh"


namespace rol::generic {

template <typename Colour>
basic_image<Colour> fill::fill_impl(const basic_image<Colour>& img) const {
    rol::basic_image<Colour> res = img.clone();
    const math::vec2u size = compute_size(_width, _height, res.size());
    const math::vec2i offset = compute_offset(_x_offset, _y_offset, size, res.size());
    rol::algo::fill_inplace(res, _colour.decay_to<Colour>(), size, offset);
    return res;
}

rgb_image fill::operator()(const rgb_image& img) const {
    return fill_impl(img);
}

greyscale_image fill::operator()(const greyscale_image& img) const {
    return fill_impl(img);
}

binary_image fill::operator()(const binary_image& img) const {
    return fill_impl(img);
}

layer fill::operator()(const layer& img) const {
    return fill_impl(img);
}

coefficient_plane fill::operator()(const coefficient_plane& img) const {
    return fill_impl(img);
}

image fill::operator()(const image& img) const {
    return std::visit([*this](const auto& img) -> image { return operator()(img); }, img);
}

} // namespace rol::generic
