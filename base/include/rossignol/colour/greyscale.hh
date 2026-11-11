
#pragma once

#include <cstdint>


namespace rol {

struct greyscale {
    uint8_t grey;

    bool operator==(const greyscale& col) const = default;
};

struct greyscalea {
    uint8_t grey;
    uint8_t a = 255;

    bool operator==(const greyscalea& col) const = default;
};

} // namespace rol
