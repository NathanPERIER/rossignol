#include "rossignol/generic/filters.hh"

#include "rossignol/algorithm/filters.hh"


namespace {

/*--------------------+
|  Low pass           |
+--------------------*/

class low_pass_impl {
public:
    low_pass_impl(const rol::coefficient_plane& img, const double& threshold): _img(img), _threshold(threshold) {}

    rol::coefficient_plane operator()(const rol::generic::sigmoid& sigmoid) const {
        rol::coefficient_plane res = _img.clone();
        rol::algo::low_pass_sigmoid_inplace(res, _threshold, sigmoid.steepness);
        return res;
    }

    rol::coefficient_plane operator()(rol::generic::detail::unit_step_t) const {
        rol::coefficient_plane res = _img.clone();
        rol::algo::low_pass_unit_step_inplace(res, _threshold);
        return res;
    }

private:
    const rol::coefficient_plane& _img;
    double _threshold;
};


/*--------------------+
|  High pass          |
+--------------------*/

class high_pass_impl {
public:
    high_pass_impl(const rol::coefficient_plane& img, const double& threshold): _img(img), _threshold(threshold) {}

    rol::coefficient_plane operator()(const rol::generic::sigmoid& sigmoid) const {
        rol::coefficient_plane res = _img.clone();
        rol::algo::high_pass_sigmoid_inplace(res, _threshold, sigmoid.steepness);
        return res;
    }

    rol::coefficient_plane operator()(rol::generic::detail::unit_step_t) const {
        rol::coefficient_plane res = _img.clone();
        rol::algo::high_pass_unit_step_inplace(res, _threshold);
        return res;
    }

private:
    const rol::coefficient_plane& _img;
    double _threshold;
};


/*--------------------+
|  Band pass          |
+--------------------*/

class band_pass_impl {
public:
    band_pass_impl(const rol::coefficient_plane& img, const double& low_threshold, const double& high_threshold):
        _img(img), _low_threshold(low_threshold), _high_threshold(high_threshold) {}

    rol::coefficient_plane operator()(const rol::generic::sigmoid& sigmoid) const {
        rol::coefficient_plane res = _img.clone();
        rol::algo::band_pass_sigmoid_inplace(res, _low_threshold, _high_threshold, sigmoid.steepness);
        return res;
    }

    rol::coefficient_plane operator()(rol::generic::detail::unit_step_t) const {
        rol::coefficient_plane res = _img.clone();
        rol::algo::band_pass_unit_step_inplace(res, _low_threshold, _high_threshold);
        return res;
    }

private:
    const rol::coefficient_plane& _img;
    double _low_threshold;
    double _high_threshold;
};

} // anonymous namespace


namespace rol::generic {

/*--------------------+
|  Low pass           |
+--------------------*/

coefficient_plane low_pass::operator()(const coefficient_plane& img) const {
    return std::visit(::low_pass_impl(img, _threshold), _method);
}

coefficient_plane low_pass::operator()(const image& img) const {
    const coefficient_plane* specific_img = std::get_if<coefficient_plane>(&img);
    if(specific_img == nullptr) {
        throw std::runtime_error("Low pass filter can only be applied on a coefficient plane ()");
    }
    return operator()(*specific_img);
}


/*--------------------+
|  High pass          |
+--------------------*/

coefficient_plane high_pass::operator()(const coefficient_plane& img) const {
    return std::visit(::high_pass_impl(img, _threshold), _method);
}

coefficient_plane high_pass::operator()(const image& img) const {
    const coefficient_plane* specific_img = std::get_if<coefficient_plane>(&img);
    if(specific_img == nullptr) {
        throw std::runtime_error("Low pass filter can only be applied on a coefficient plane ()");
    }
    return operator()(*specific_img);
}


/*--------------------+
|  Band pass          |
+--------------------*/

coefficient_plane band_pass::operator()(const coefficient_plane& img) const {
    return std::visit(::band_pass_impl(img, _low_threshold, _high_threshold), _method);
}

coefficient_plane band_pass::operator()(const image& img) const {
    const coefficient_plane* specific_img = std::get_if<coefficient_plane>(&img);
    if(specific_img == nullptr) {
        throw std::runtime_error("Low pass filter can only be applied on a coefficient plane ()");
    }
    return operator()(*specific_img);
}

} // namespace rol::generic
