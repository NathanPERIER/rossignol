

#pragma once

#include "rossignol/image/generic.hh"
#include "rossignol/utils/errors/check.hh"


namespace rol::generic {

class extract_bit_plane {
public:
    extract_bit_plane(uint8_t idx): _index(idx) {
        ROL_CHECK(_index < 8, "Index for bit plane extraction should be between 0 and 7, got {}", idx);
    }

    binary_image operator()(const layer& img) const;

    binary_image operator()(const image& img) const;

private:
    uint8_t _index;

    binary_image operator()(const rgb_image&) const;
    binary_image operator()(const greyscale_image&) const;
    binary_image operator()(const binary_image&) const;
    binary_image operator()(const coefficient_plane&) const;
};

} // namespace rol::generic
