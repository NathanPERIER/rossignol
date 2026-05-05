
#pragma once

#include <algorithm>
#include <cstdint>
#include <span>
#include <variant>
#include <vector>

#include "rossignol/colour/rgb.hh"
#include "rossignol/colour/greyscale.hh"


namespace rol {

struct image_size {
    std::size_t width;
    std::size_t height;
};

template <typename Colour>
class basic_image {
public:
    basic_image(image_size size);

    /// @brief creates a plain image filled with a given colour
    basic_image(image_size size, Colour fill);

    basic_image(basic_image<Colour>&&) = default;
    basic_image<Colour>& operator=(basic_image<Colour>&&) = default;

    /// @brief creates a new image that points to the same underlying buffer as the current image
    basic_image<Colour> share() const { return basic_image<Colour>(*this); }

    /// @brief creates copy of the current image
    basic_image<Colour> clone() const;

    const image_size& size() const { return _size; }
    std::size_t width() const { return _size.width; }
    std::size_t height() const { return _size.height; }

    Colour& at(std::size_t y, std::size_t x) { return _rows[y][x]; }
    const Colour& at(std::size_t y, std::size_t x) const { return _rows[y][x]; }

    std::span<Colour> operator[](std::size_t y) { return _rows[y]; }
    std::span<const Colour> operator[](std::size_t y) const { return _rows[y]; }

    void invert_y() { std::ranges::reverse(_rows); }

private:
    image_size _size;
    std::shared_ptr<Colour[]> _pixels;
    std::vector<std::span<Colour>> _rows;

    basic_image(const basic_image<Colour>&) = default;
    basic_image<Colour>& operator=(const basic_image<Colour>&) = default;
};


using rgb_image = basic_image<rgba>;
using greyscale_image = basic_image<greyscalea>;
using binary_image = basic_image<bool>;
using layer = basic_image<uint8_t>;
using coefficient_plane = basic_image<double>;

using image = std::variant<rgb_image, greyscale_image, binary_image, layer, coefficient_plane>;


image parse_image(std::span<const uint8_t> raw);

std::vector<uint8_t> dump_image(image& img);

} // namespace rol
