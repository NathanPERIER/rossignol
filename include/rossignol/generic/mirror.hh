
#pragma once

#include "rossignol/image/image.hh"


namespace rol::generic {

enum class mirror_direction {
    horizontal,
    vertical
};

class mirror {
public:
    mirror(mirror_direction direction): _direction(direction) {}

    template <typename Colour>
    basic_image<Colour> operator()(const basic_image<Colour>& img) const;

    image operator()(const image& img) const;

private:
    mirror_direction _direction;
};

} // namespace rol::generic
