
#pragma once

#include <cstdint>


namespace rol {

struct greyscalea {
    uint8_t grey;
    uint8_t a = 255;

    bool operator==(const greyscalea& col) const = default;
};

} // namespace rol
