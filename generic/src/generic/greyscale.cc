#include "rossignol/generic/greyscale.hh"

#include "rossignol/algorithm/greyscale.hh"


namespace {

class greyscale_method_impl {
public:
    greyscale_method_impl(const rol::rgb_image& img): _img(img) {}

    rol::greyscale_image operator()(const rol::generic::luma& luma) const {
        switch(luma.method) {
            case rol::luma_method::sdtv: return rol::algo::greyscale_luma<rol::luma_method::sdtv>(_img);
            case rol::luma_method::hdtv: return rol::algo::greyscale_luma<rol::luma_method::hdtv>(_img);
            case rol::luma_method::hdr:  return rol::algo::greyscale_luma<rol::luma_method::hdr>(_img);
            default: throw std::runtime_error("Unsupported luma method");
        }
    }
    rol::greyscale_image operator()(rol::generic::detail::intensity_t) const {
        return rol::algo::greyscale_intensity(_img);
    }
    rol::greyscale_image operator()(rol::generic::detail::lightness_t) const {
        return rol::algo::greyscale_lightness(_img);
    }
    rol::greyscale_image operator()(rol::generic::detail::value_t) const {
        return rol::algo::greyscale_value(_img);
    }

private:
    const rol::rgb_image& _img;
};

} // anonymous namespace


namespace rol::generic {

greyscale_image to_greyscale::operator()(const rgb_image& img) const {
    return std::visit(::greyscale_method_impl(img), _method);
}

greyscale_image to_greyscale::operator()(const binary_image& img) const {
    return rol::algo::greyscale(img);
}

greyscale_image to_greyscale::operator()(const layer& img) const {
    return rol::algo::greyscale(img);
}

greyscale_image to_greyscale::operator()(const coefficient_plane& img) const {
    return rol::algo::greyscale(img);
}

greyscale_image to_greyscale::operator()(const image& img) const {
    return std::visit(*this, img);
}

} // namespace rol::generic
