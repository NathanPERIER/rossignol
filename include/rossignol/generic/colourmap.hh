
#pragma once

#include "rossignol/colour/colourmap.hh"
#include "rossignol/image/image.hh"


namespace rol::generic {

class apply_colourmap {
public:
    apply_colourmap(std::shared_ptr<const colourmap> cmap): _colourmap(cmap) {
        if(_colourmap == nullptr) [[unlikely]] {
            throw std::runtime_error("Colourmap cannot be null");
        }
    }

    rgb_image operator()(const greyscale_image& img) const;
    rgb_image operator()(const layer& img) const;
    rgb_image operator()(const coefficient_plane& img) const;

    rgb_image operator()(const image& img) const;

private:
    std::shared_ptr<const colourmap> _colourmap;

    rgb_image operator()(const rgb_image&) const;
    rgb_image operator()(const binary_image&) const;
};

} // namespace rol::generic
