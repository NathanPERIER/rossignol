#include "rossignol/algorithm/filters.hh"

#include "rossignol/algorithm/map_pixels.hh"
#include "rossignol/math/arithmetic/logistic_function.hh"


namespace {

enum class filter_impl_type {
    low_pass,
    high_pass,
    band_pass,
    band_stop
};

/*--------------------+
|  Unit step          |
+--------------------*/

/// @brief helper class for applying a unit step filter function
template <filter_impl_type Filter>
class unit_step_filter_impl;

template <>
class unit_step_filter_impl<filter_impl_type::low_pass> {
public:
    unit_step_filter_impl(const rol::params::low_pass_filter& filter): _filter(filter) {}

    double operator()(double pixel) const {
        return (pixel < _filter.threshold) ? 0.0 : 1.0;
    }

private:
    rol::params::low_pass_filter _filter;
};

template <>
class unit_step_filter_impl<filter_impl_type::high_pass> {
public:
    unit_step_filter_impl(const rol::params::high_pass_filter& filter): _filter(filter) {}

    double operator()(double pixel) const {
        return (pixel > _filter.threshold) ? 0.0 : 1.0;
    }

private:
    rol::params::high_pass_filter _filter;
};

template <>
class unit_step_filter_impl<filter_impl_type::band_pass> {
public:
    unit_step_filter_impl(const rol::params::band_pass_filter& filter): _filter(filter) {}

    double operator()(double pixel) const {
        return (pixel < _filter.low_threshold || _filter.high_threshold < pixel) ? 0.0 : 1.0;
    }

private:
    rol::params::band_pass_filter _filter;
};

template <>
class unit_step_filter_impl<filter_impl_type::band_stop> {
public:
    unit_step_filter_impl(const rol::params::band_pass_filter& filter): _filter(filter) {}

    double operator()(double pixel) const {
        return (_filter.high_threshold < pixel && pixel < _filter.low_threshold) ? 0.0 : 1.0;
    }

private:
    rol::params::band_pass_filter _filter;
};

// Could we use a concept for FilterConsumer ?
template <typename FilterConsumer>
auto with_unit_step_filter(const rol::params::coefficient_filter& filter, const FilterConsumer& consumer) {
    struct filter_visitor {
    public:
        filter_visitor(const FilterConsumer& consumer): _consumer(consumer) {}

        auto operator()(const rol::params::low_pass_filter& filter) const {
            return _consumer(unit_step_filter_impl<filter_impl_type::low_pass>(filter));
        }

        auto operator()(const rol::params::high_pass_filter& filter) const {
            return _consumer(unit_step_filter_impl<filter_impl_type::high_pass>(filter));
        }

        auto operator()(const rol::params::band_pass_filter& filter) const {
            if(filter.high_threshold < filter.low_threshold) {
                return _consumer(unit_step_filter_impl<filter_impl_type::band_stop>(filter));
            } else {
                return _consumer(unit_step_filter_impl<filter_impl_type::band_pass>(filter));
            }
        }

    private:
        const FilterConsumer& _consumer;
    };

    return std::visit(filter_visitor(consumer), filter);
}


/*--------------------+
|  Sigmoid            |
+--------------------*/

constexpr double sigmoid_steepness_factor = 128.0;

/// @brief helper class for applying a unit step filter function
template <filter_impl_type Filter>
class sigmoid_filter_impl;

template <>
class sigmoid_filter_impl<filter_impl_type::low_pass> {
public:
    sigmoid_filter_impl(const rol::params::low_pass_filter& filter, const double& steepness): _sigmoid(filter.threshold, steepness * ::sigmoid_steepness_factor) {}

    double operator()(double pixel) const {
        return 1.0 - _sigmoid(pixel);
    }

private:
    rol::math::logistic_function _sigmoid;
};

template <>
class sigmoid_filter_impl<filter_impl_type::high_pass> {
public:
    sigmoid_filter_impl(const rol::params::high_pass_filter& filter, const double& steepness): _sigmoid(filter.threshold, steepness * ::sigmoid_steepness_factor) {}

    double operator()(double pixel) const {
        return _sigmoid(pixel);
    }

private:
    rol::math::logistic_function _sigmoid;
};

template <>
class sigmoid_filter_impl<filter_impl_type::band_pass> {
public:
    sigmoid_filter_impl(const rol::params::band_pass_filter& filter, const double& steepness):
        _sigmoid_low(filter.low_threshold, steepness * ::sigmoid_steepness_factor),
        _sigmoid_high(filter.high_threshold, steepness * ::sigmoid_steepness_factor) {}

    double operator()(double pixel) const {
        return _sigmoid_low(pixel) * (1.0 - _sigmoid_high(pixel));
    }

private:
    rol::math::logistic_function _sigmoid_low;
    rol::math::logistic_function _sigmoid_high;
};

template <>
class sigmoid_filter_impl<filter_impl_type::band_stop> {
public:
    sigmoid_filter_impl(const rol::params::band_pass_filter& filter, const double& steepness):
        _sigmoid_low(filter.low_threshold, steepness * ::sigmoid_steepness_factor),
        _sigmoid_high(filter.high_threshold, steepness * ::sigmoid_steepness_factor) {}

    double operator()(double pixel) const {
        return 1.0 - _sigmoid_high(pixel) * (1.0 - _sigmoid_low(pixel));
    }

private:
    rol::math::logistic_function _sigmoid_low;
    rol::math::logistic_function _sigmoid_high;
};

// Could we use a concept for FilterConsumer ?
template <typename FilterConsumer>
auto with_sigmoid_filter(const rol::params::coefficient_filter& filter, const double& steepness, const FilterConsumer& consumer) {
    struct filter_visitor {
    public:
        filter_visitor(const double& steepness, const FilterConsumer& consumer): _steepness(steepness), _consumer(consumer) {}

        auto operator()(const rol::params::low_pass_filter& filter) const {
            return _consumer(sigmoid_filter_impl<filter_impl_type::low_pass>(filter, _steepness));
        }

        auto operator()(const rol::params::high_pass_filter& filter) const {
            return _consumer(sigmoid_filter_impl<filter_impl_type::high_pass>(filter, _steepness));
        }

        auto operator()(const rol::params::band_pass_filter& filter) const {
            if(filter.high_threshold < filter.low_threshold) {
                return _consumer(sigmoid_filter_impl<filter_impl_type::band_stop>(filter, _steepness));
            } else {
                return _consumer(sigmoid_filter_impl<filter_impl_type::band_pass>(filter, _steepness));
            }
        }

    private:
        const double& _steepness;
        const FilterConsumer& _consumer;
    };

    return std::visit(filter_visitor(steepness, consumer), filter);
}

} // anonymous namespace


namespace rol::algo {

/*--------------------+
|  Unit step          |
+--------------------*/

void unit_step_filter_inplace(coefficient_plane& img, const params::coefficient_filter& filter) {
    ::with_unit_step_filter(filter, [&img]<rol::yield_invocable<double, double> FilterImpl>(const FilterImpl& filter) {
        edit_pixels_inplace(img, [&filter](double& pixel) { pixel *= filter(pixel); });
    });
}


/*--------------------+
|  Sigmoid            |
+--------------------*/

void sigmoid_filter_inplace(coefficient_plane& img, const params::coefficient_filter& filter, double steepness) {
    ::with_sigmoid_filter(filter, steepness, [&img]<rol::yield_invocable<double, double> FilterImpl>(const FilterImpl& filter) {
        edit_pixels_inplace(img, [&filter](double& pixel) { pixel *= filter(pixel); });
    });
}

} // namespace rol::algo
