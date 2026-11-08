

#pragma once

#include "rossignol/image/image.hh"


namespace rol::generic {

class extract_bit_plane {
public:
    extract_bit_plane(uint8_t idx): _index(idx) {
        if(_index >= 8) {
            throw std::runtime_error("Bad index for bit plane extraction (should be between 0 and 7)");
        }
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
