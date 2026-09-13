#include "rossignol/generic/filters.hh"

#include "rossignol/algorithm/filters.hh"


namespace {

class coefficient_filter_impl {
public:
    coefficient_filter_impl(const rol::params::coefficient_filter& filter, const rol::coefficient_plane& img): _filter(filter), _img(img) {}

    rol::coefficient_plane operator()(const rol::generic::detail::unit_step_t&) const {
        rol::coefficient_plane res = _img.clone();
        rol::algo::unit_step_filter_inplace(res, _filter);
        return res;
    }

    rol::coefficient_plane operator()(const rol::generic::sigmoid& sigmoid) const {
        rol::coefficient_plane res = _img.clone();
        rol::algo::sigmoid_filter_inplace(res, _filter, sigmoid.steepness);
        return res;
    }

private:
    const rol::params::coefficient_filter& _filter;
    const rol::coefficient_plane& _img;
};

class coefficient_filter_mask_impl {
public:
    coefficient_filter_mask_impl(const rol::params::coefficient_filter& filter, const rol::coefficient_plane& img): _filter(filter), _img(img) {}

    rol::coefficient_plane operator()(const rol::generic::detail::unit_step_t&) const {
        return rol::algo::unit_step_filter_mask(_img, _filter);
    }

    rol::coefficient_plane operator()(const rol::generic::sigmoid& sigmoid) const {
        return rol::algo::sigmoid_filter_mask(_img, _filter, sigmoid.steepness);
    }

private:
    const rol::params::coefficient_filter& _filter;
    const rol::coefficient_plane& _img;
};

} // anonymous namespace


namespace rol::generic {

coefficient_plane coefficient_filter::operator()(const rgb_image&) const {
    throw std::runtime_error("Coefficient filtering is not available for RGB images (maybe extract a coefficient first ?)");
}

coefficient_plane coefficient_filter::operator()(const greyscale_image&) const {
    throw std::runtime_error("Coefficient filtering is not available for greyscale images (maybe extract a coefficient first ?)");
}

coefficient_plane coefficient_filter::operator()(const layer&) const {
    throw std::runtime_error("Coefficient filtering is not available for unnamed channels (maybe convert to a coefficient first ?)");
}

coefficient_plane coefficient_filter::operator()(const coefficient_plane& img) const {
    return std::visit(::coefficient_filter_impl(_filter, img), _method);
}

coefficient_plane coefficient_filter::operator()(const binary_image&) const {
    throw std::runtime_error("Coefficient filtering is not available for binary images (maybe convert to a coefficient first ?)");
}

coefficient_plane coefficient_filter::operator()(const image& img) const {
    return std::visit([this](const auto& img) -> coefficient_plane { return operator()(img); }, img);
}


coefficient_plane coefficient_filter_mask::operator()(const rgb_image&) const {
    throw std::runtime_error("Coefficient filtering is not available for RGB images (maybe extract a coefficient first ?)");
}

coefficient_plane coefficient_filter_mask::operator()(const greyscale_image&) const {
    throw std::runtime_error("Coefficient filtering is not available for greyscale images (maybe extract a coefficient first ?)");
}

coefficient_plane coefficient_filter_mask::operator()(const layer&) const {
    throw std::runtime_error("Coefficient filtering is not available for unnamed channels (maybe convert to a coefficient first ?)");
}

coefficient_plane coefficient_filter_mask::operator()(const coefficient_plane& img) const {
    return std::visit(::coefficient_filter_mask_impl(_filter, img), _method);
}

coefficient_plane coefficient_filter_mask::operator()(const binary_image&) const {
    throw std::runtime_error("Coefficient filtering is not available for binary images (maybe convert to a coefficient first ?)");
}

coefficient_plane coefficient_filter_mask::operator()(const image& img) const {
    return std::visit([this](const auto& img) -> coefficient_plane { return operator()(img); }, img);
}

} // namespace rol::generic
