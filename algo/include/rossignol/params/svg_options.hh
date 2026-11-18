
#pragma once


namespace rol::params {

struct svg_options {
    bool bounding_box = false;
    bool pretty = false;
};

} // namespace rol::params


namespace rol::defaults {

constexpr rol::params::svg_options svg_options {};

} // namespace rol::defaults
