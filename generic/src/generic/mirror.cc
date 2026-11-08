#include "rossignol/generic/mirror.hh"

#include "rossignol/algorithm/mirror.hh"


namespace rol::generic {

template <typename Colour>
basic_image<Colour> mirror::operator()(const basic_image<Colour>& img) const {
    switch(_direction) {
        case mirror_direction::horizontal: return rol::algo::horizontal_mirror(img);
        case mirror_direction::vertical:   return rol::algo::vertical_mirror(img);
        default: throw std::runtime_error("Unsupported mirror direction");
    }
}

image mirror::operator()(const image& img) const {
    const auto visitor = [this]<typename Colour>(const basic_image<Colour>& img) -> image {
        return operator()(img);
    };
    return std::visit(visitor, img);
}

} // namespace rol::generic
