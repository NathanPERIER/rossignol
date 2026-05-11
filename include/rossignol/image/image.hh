
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
    basic_image(image_size size): _size(size), _pixels(std::make_shared<Colour[]>(_size.width * _size.height)), _rows(std::make_shared<std::vector<std::span<Colour>>>()) {
        _rows->reserve(_size.height);
        for(std::size_t y = 0; y < _size.height; y++) {
            _rows->push_back(std::span<Colour>(_pixels.get() + _size.width * y, _size.width));
        }
    }

    /// @brief creates a plain image filled with a given colour
    basic_image(image_size size, Colour fill): basic_image(size) {
        for(const std::span<Colour> row: *_rows) {
            std::fill(row.begin(), row.end(), fill);
        }
    }

    basic_image(basic_image<Colour>&&) = default;
    basic_image<Colour>& operator=(basic_image<Colour>&&) = default;

    /// @brief creates a new image that points to the same underlying buffer as the current image
    basic_image<Colour> share() const { return basic_image<Colour>(*this); }

    /// @brief creates copy of the current image
    basic_image<Colour> clone() const {
        basic_image<Colour> res(_size);
        for(std::size_t y = 0; y < _size.height; y++) {
            const std::span<const Colour> row = (*_rows)[y];
            std::copy(row.begin(), row.end(), res[y].begin());
        }
        return res;
    }

    const image_size& size() const { return _size; }
    std::size_t width() const { return _size.width; }
    std::size_t height() const { return _size.height; }

    Colour& at(std::size_t y, std::size_t x) { return (*_rows)[y][x]; }
    const Colour& at(std::size_t y, std::size_t x) const { return (*_rows)[y][x]; }

    std::span<Colour> operator[](std::size_t y) { return (*_rows)[y]; }
    std::span<const Colour> operator[](std::size_t y) const { return (*_rows)[y]; }

    void invert_y() {
        std::shared_ptr<std::vector<std::span<Colour>>> rows = std::make_shared<std::vector<std::span<Colour>>>();
        rows->reserve(_rows->size());
        for(auto it = _rows->rbegin(); it != _rows->rend(); ++it) {
            rows->push_back(*it);
        }
        _rows = std::move(rows);
    }

private:
    image_size _size;
    std::shared_ptr<Colour[]> _pixels;
    std::shared_ptr<std::vector<std::span<Colour>>> _rows;

    basic_image(const basic_image<Colour>&) = default;
    basic_image<Colour>& operator=(const basic_image<Colour>&) = default;
};


using rgb_image = basic_image<rgba>;
using greyscale_image = basic_image<greyscalea>;
using binary_image = basic_image<bool>;
using layer = basic_image<uint8_t>;
using coefficient_plane = basic_image<double>;

using image = std::variant<rgb_image, greyscale_image, binary_image, layer, coefficient_plane>;

} // namespace rol
