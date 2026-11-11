
#pragma once

#include "rossignol/image/generic.hh"
#include "rossignol/params/generic/prepare_optims.hh"


namespace rol::generic {

/// @brief converts an image to a format suitable for writing when necessary and applies some optimisations
class prepare_write {
public:
    prepare_write(prepare_colour_optims colour_optims = defaults::prepare_colour_optims): _colour_optims(colour_optims) {}

    image operator()(const rgb_image&) const;
    greyscale_image operator()(const greyscale_image&) const;
    greyscale_image operator()(const layer& img) const;
    greyscale_image operator()(const coefficient_plane& img) const;
    greyscale_image operator()(const binary_image&) const;

    image operator()(const image& img) const;

private:
    prepare_colour_optims _colour_optims;
};

} // namespace rol::generic
