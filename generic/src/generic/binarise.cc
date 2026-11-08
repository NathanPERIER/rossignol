#include "rossignol/generic/binarise.hh"

#include "rossignol/algorithm/binarise.hh"


namespace {

class binarise_greyscale_impl {
public:
    binarise_greyscale_impl(const rol::greyscale_image& img): _img(img) {}

    rol::binary_image operator()(rol::generic::floyd_steinberg_t) {
        return rol::algo::floyd_steinberg(_img);
    }
    rol::binary_image operator()(const rol::generic::binary_threshold& threshold) {
        return rol::algo::binarise_threshold(_img, threshold.value);
    }

private:
    const rol::greyscale_image& _img;
};

} // anonymous namespace


namespace rol::generic {

binary_image binarise::operator()(const rgb_image&) const {
    throw std::runtime_error("Unable to binarise an RGB image, convert it to greyscale first");
}

binary_image binarise::operator()(const greyscale_image& img) const {
    return std::visit(::binarise_greyscale_impl(img), _method);
}

binary_image binarise::operator()(const binary_image& img) const {
    return img.share();
}

binary_image binarise::operator()(const layer&) const {
    throw std::runtime_error("Unable to binarise an unnamed layer, convert it to greyscale first");
}

binary_image binarise::operator()(const coefficient_plane&) const {
    throw std::runtime_error("Unable to binarise a coefficient plane (maybe convert to greyscale ?)");
}

binary_image binarise::operator()(const image& img) const {
    return std::visit([this](const auto& img) -> binary_image { return operator()(img); }, img);
}

} // namespace rol::generic
