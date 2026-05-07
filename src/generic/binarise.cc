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

class binarise_general_impl {
public:
    binarise_general_impl(const rol::generic::binarise& op): _op(op) {}

    rol::binary_image operator()(const rol::binary_image& img) {
        return _op(img);
    }
    rol::binary_image operator()(const rol::rgb_image&) {
        throw std::runtime_error("Unable to binarise an RGB image, convert it to greyscale first");
    }
    rol::binary_image operator()(const rol::layer&) {
        throw std::runtime_error("Unable to binarise an unnamed layer, convert it to greyscale first");
    }
    rol::binary_image operator()(const rol::coefficient_plane&) {
        throw std::runtime_error("Unable to binarise a coefficient plane (maybe convert to greyscale ?)");
    }
    rol::binary_image operator()(const rol::greyscale_image& img) {
        return _op(img);
    }

private:
    const rol::generic::binarise& _op;
};

} // anonymous namespace


namespace rol::generic {

binary_image binarise::operator()(const greyscale_image& img) const {
    return std::visit(::binarise_greyscale_impl(img), _method);
}

binary_image binarise::operator()(const binary_image& img) const {
    return img.share();
}

binary_image binarise::operator()(const image& img) const {
    return std::visit(::binarise_general_impl(*this), img);
}

} // namespace rol::generic
