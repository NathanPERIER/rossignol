
#pragma once

#include "rossignol/image/image.hh"


namespace rol::generic {

class to_coefficients {
public:
    to_coefficients() = default;

    coefficient_plane operator()(const layer& img) const;
    coefficient_plane operator()(const coefficient_plane& img) const { return img.share(); }

    coefficient_plane operator()(const image& img) const;

private:
    coefficient_plane operator()(const rgb_image&) const;
    coefficient_plane operator()(const greyscale_image&) const;
    coefficient_plane operator()(const binary_image&) const;
};

} // namespace rol::generic
