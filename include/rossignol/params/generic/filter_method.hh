
#pragma once

#include <variant>

#include "rossignol/params/filters.hh"


namespace rol::generic::detail {

struct unit_step_t {};

} // namespace rol::generic::detail


namespace rol::generic {

constexpr detail::unit_step_t unit_step;

struct sigmoid {
    double steepness = rol::defaults::sigmoid_steepness;
};

using filter_method = std::variant<detail::unit_step_t, sigmoid>;

} // namespace rol::generic


namespace rol::generic::defaults {

constexpr rol::generic::filter_method filter_method = rol::generic::unit_step;

} // namespace rol::generic::defaults
