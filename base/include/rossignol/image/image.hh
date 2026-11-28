
#pragma once

#include "rossignol/colour/rgb.hh"
#include "rossignol/colour/greyscale.hh"
#include "rossignol/image/pixel_buffer.hh"


namespace rol {

template <typename Colour>
class basic_image {
public:
    basic_image(const math::vec2u& size): basic_image(std::make_shared<buffer_type>(size)) {}

    /// @brief creates a plain image filled with a given colour
    basic_image(const math::vec2u& size, Colour fill): basic_image(size) {
        for(std::size_t y = 0; y < height(); y++) {
            std::ranges::fill((*this)[y], fill);
        }
    }

    basic_image(basic_image<Colour>&&) = default;
    basic_image<Colour>& operator=(basic_image<Colour>&&) = default;

    /// @brief creates a new image that points to the same underlying buffer as the current image
    basic_image<Colour> share() { return basic_image<Colour>(*this); }

    basic_image<Colour> sub_image(const math::vec2u& offset, const math::vec2u& size) {
        return basic_image<Colour>(_buffer->sub_buffer(offset, size));
    }

    /// @brief creates copy of the current image
    basic_image<Colour> clone() const {
        return basic_image<Colour>(std::make_shared<buffer_type>(_buffer->clone()));
    }

    const math::vec2u& size() const { return _buffer->size(); }
    std::size_t width() const { return _buffer->width(); }
    std::size_t height() const { return _buffer->height(); }

    std::span<Colour> operator[](std::size_t y) { return (*_buffer)[y]; }
    std::span<const Colour> operator[](std::size_t y) const { return (*_buffer)[y]; }

    void invert_y() {
        _buffer = std::make_shared<buffer_type>(*_buffer);
        _buffer->invert_y();
    }

private:
    using buffer_type = pixel_buffer<Colour>;
    std::shared_ptr<buffer_type> _buffer;

    basic_image(std::shared_ptr<buffer_type>&& buf): _buffer(std::move(buf)) {}

    basic_image(const basic_image<Colour>&) = default;
    basic_image<Colour>& operator=(const basic_image<Colour>&) = default;
};


using rgb_image = basic_image<rgba>;
using greyscale_image = basic_image<greyscalea>;
using binary_image = basic_image<bool>;
using layer = basic_image<uint8_t>;
using coefficient_plane = basic_image<double>;

} // namespace rol
