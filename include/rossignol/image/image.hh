
#pragma once

#include <cstdint>
#include <span>
#include <variant>
#include <vector>

#include "rossignol/colour/rgb.hh"
#include "rossignol/colour/greyscale.hh"


namespace rol {

template <typename Colour>
class basic_image {
public:
    basic_image(std::size_t width, std::size_t height);

    std::size_t width() const { return _width; }
    std::size_t height() const { return _height; }

    Colour& at(std::size_t y, std::size_t x) { return _rows[y][x]; }
    const Colour& at(std::size_t y, std::size_t x) const { return _rows[y][x]; }

    std::span<Colour> operator[](std::size_t y) { return _rows[y]; }
    std::span<const Colour> operator[](std::size_t y) const { return _rows[y]; }

private:
    std::size_t _width;
    std::size_t _height;
    std::unique_ptr<Colour[]> _pixels;
    std::vector<std::span<Colour>> _rows;
};


using rgb_image = basic_image<rgba>;
using greyscale_image = basic_image<greyscalea>;
using binary_image = basic_image<bool>;

using image = std::variant<rgb_image, greyscale_image, binary_image>;

} // namespace rol
