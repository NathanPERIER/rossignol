#include "rossignol/image/algorithm/mirror.hh"


namespace {

class mirror_impl {
public:
    mirror_impl(rol::mirror_direction direction): _direction(direction) {}

    template <typename Colour>
    rol::image operator()(const rol::basic_image<Colour>& img) {
        switch(_direction) {
            case rol::mirror_direction::horizontal: return rol::detail::horizontal_mirror(img);
            case rol::mirror_direction::vertical:   return rol::detail::vertical_mirror(img);
            default: throw std::runtime_error("Unsupported mirror direction");
        }
    }

private:
    rol::mirror_direction _direction;
};

} // anonymous namespace


namespace rol {

image mirror(const image& img, mirror_direction direction) {
    return std::visit(mirror_impl(direction), img);
}

} // namespace rol
