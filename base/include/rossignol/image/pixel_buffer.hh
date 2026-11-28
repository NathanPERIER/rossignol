
#pragma once

#include <algorithm>
#include <memory>
#include <span>
#include <vector>

#include "rossignol/math/vec2.hh"
#include "rossignol/utils/errors/check.hh"


namespace rol {

template <typename Colour>
class pixel_buffer {
public:
    pixel_buffer(const math::vec2u& size): _size(size), _data(std::make_shared<Colour[]>(_size.x * _size.y)) {
        ROL_CHECK(_size.x != 0 && _size.y != 0, "An image cannot be empty");
        _rows.reserve(_size.y);
        for(std::size_t y = 0; y < _size.y; y++) {
            _rows.push_back(std::span<Colour>(_data.get() + _size.x * y, _size.x));
        }
    }

    pixel_buffer<Colour> clone() const {
        pixel_buffer<Colour> res(_size);
        for(std::size_t y = 0; y < _size.y; y++) {
            std::ranges::copy((*this)[y], res[y].begin());
        }
        return res;
    }

    std::shared_ptr<pixel_buffer<Colour>> sub_buffer(const math::vec2u& offset, const math::vec2u& size) {
        return std::make_shared<pixel_buffer<Colour>>(*this, offset, size);
    }

    std::shared_ptr<const pixel_buffer<Colour>> sub_buffer(const math::vec2u& offset, const math::vec2u& size) const {
        return std::make_shared<pixel_buffer<Colour>>(*this, offset, size);
    }

    const math::vec2u& size() const { return _size; }
    std::size_t width() const { return _size.x; }
    std::size_t height() const { return _size.y; }

    std::span<const Colour> at(std::size_t y) const { return _rows.at(y); }
    std::span<Colour> at(std::size_t y) { return _rows.at(y); }

    std::span<const Colour> operator[](std::size_t y) const { return _rows[y]; }
    std::span<Colour> operator[](std::size_t y) { return _rows[y]; }

    void invert_y() {
        std::reverse(_rows.begin(), _rows.end());
    }

private:
    math::vec2u _size;
    std::shared_ptr<Colour[]> _data;
    std::vector<std::span<Colour>> _rows;

    pixel_buffer(const pixel_buffer<Colour>& buf, const math::vec2u& offset, const math::vec2u& size): _data(buf._data) {
        ROL_CHECK(size.x != 0 && size.y != 0, "An image cannot be empty");
        if(offset.x > buf.width() || offset.y > buf.height()) {
            throw rol::input_error("Sub-image is out of bounds");
        }
        const std::size_t last_y = std::min(buf.height(), offset.y + size.y);
        _size = math::vec2u {
            .x = std::min(buf.width(), offset.x + size.x) - offset.x,
            .y = last_y - offset.y
        };
        _rows->reserve(_size.y);
        for(std::size_t y = offset.y; y < last_y; y++) {
            _rows->push_back(buf[y].subspan(offset.x, _size.x));
        }
    }
};

} // namespace rol
