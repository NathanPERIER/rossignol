#include "rossignol/image/image.hh"


namespace rol {

template <typename Colour>
basic_image<Colour>::basic_image(std::size_t width, std::size_t height): _width(width), _height(height), _pixels(std::make_unique<Colour[]>(_width * _height)) {
    _rows.reserve(height);
    for(std::size_t y = 0; y < _height; y++) {
        _rows.push_back(std::span<Colour>(_pixels.get() + _width * y, _width));
    }
}

} // namespace rol
