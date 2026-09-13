
#pragma once

#include "rossignol/image/image.hh"
#include "rossignol/params/filters.hh"
#include "rossignol/params/generic/filter_method.hh"


namespace rol::generic {

class coefficient_filter {
public:
    coefficient_filter(const params::coefficient_filter& filter, filter_method method = defaults::filter_method): _filter(filter), _method(method) {}

    coefficient_plane operator()(const coefficient_plane& img) const;

    coefficient_plane operator()(const image& img) const;

private:
    params::coefficient_filter _filter;
    filter_method _method;

    coefficient_plane operator()(const rgb_image&) const;
    coefficient_plane operator()(const greyscale_image&) const;
    coefficient_plane operator()(const layer&) const;
    coefficient_plane operator()(const binary_image&) const;
};

} // namespace rol::generic
