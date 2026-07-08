
#pragma once

#include "rossignol/image/image.hh"
#include "rossignol/params/generic/filter_method.hh"


namespace rol::generic {

class low_pass {
public:
    low_pass(double threshold, filter_method method = defaults::filter_method): _threshold(threshold), _method(method) {}

    coefficient_plane operator()(const coefficient_plane& img) const;

    coefficient_plane operator()(const image& img) const;

private:
    double _threshold;
    filter_method _method;
};


class high_pass {
public:
    high_pass(double threshold, filter_method method = defaults::filter_method): _threshold(threshold), _method(method) {}

    coefficient_plane operator()(const coefficient_plane& img) const;

    coefficient_plane operator()(const image& img) const;

private:
    double _threshold;
    filter_method _method;
};


class band_pass {
public:
    band_pass(double low_threshold, double high_threshold, filter_method method = defaults::filter_method):
        _low_threshold(low_threshold), _high_threshold(high_threshold), _method(method) {}

    coefficient_plane operator()(const coefficient_plane& img) const;

    coefficient_plane operator()(const image& img) const;

private:
    double _low_threshold;
    double _high_threshold;
    filter_method _method;
};

} // namespace rol::generic
