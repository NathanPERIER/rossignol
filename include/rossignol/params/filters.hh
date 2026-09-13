
#pragma once

#include <variant>


namespace rol::params {

struct low_pass_filter {
    double threshold;
};

struct high_pass_filter {
    double threshold;
};

struct band_pass_filter {
    double low_threshold;
    double high_threshold;
};

using coefficient_filter = std::variant<low_pass_filter, high_pass_filter, band_pass_filter>;

} // namespace rol::params


namespace rol::defaults {

constexpr double sigmoid_steepness = 1.0;

} // namespace rol::defaults
