#include "rossignol/generic/prepare_write.hh"

#include "rossignol/algorithm/greyscale.hh"
#include "rossignol/algorithm/map_pixels.hh"


namespace {

bool is_only_greyscale(const rol::rgb_image& img) {
    for(std::size_t y = 0; y < img.height(); y++) {
        for(const rol::rgba& pixel: img[y]) {
            if(pixel.r != pixel.g || pixel.g != pixel.b) {
                return false;
            }
        }
    }
    return true;
}

} // anonymous namespace


namespace rol::generic {

image prepare_write::operator()(const rgb_image& img) const {
    // TODO remove alpha channel if not needed
    if(_colour_optims != prepare_colour_optims::greyscale || !::is_only_greyscale(img)) {
        // TODO maybe share if nothing to do ?
        return img.clone();
    }
    return rol::algo::map_pixels<greyscalea>(img, [](const rgba& pixel) {
        return greyscalea {
            .grey = pixel.r,
            .a = pixel.a
        };
    });
}

greyscale_image prepare_write::operator()(const greyscale_image& img) const {
    // TODO maybe share if nothing to do ?
    // TODO remove alpha channel if not needed
    return img.clone();
}

greyscale_image prepare_write::operator()(const layer& img) const {
    return rol::algo::greyscale(img);
}

greyscale_image prepare_write::operator()(const coefficient_plane& img) const {
    return rol::algo::greyscale(img);
}

greyscale_image prepare_write::operator()(const binary_image& img) const {
    return rol::algo::greyscale(img);
}

image prepare_write::operator()(const image& img) const {
    return std::visit([this](const auto& img) -> image { return operator()(img); }, img);
}

} // namespace rol::generic
