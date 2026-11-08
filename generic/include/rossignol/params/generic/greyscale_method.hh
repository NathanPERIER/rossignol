
#pragma once

#include <variant>

#include "rossignol/params/luma_method.hh"


namespace rol::generic::detail {

struct intensity_t {};
struct lightness_t {};
struct value_t {};

} // namespace rol::generic::detail


namespace rol::generic {

struct luma {
    constexpr luma(luma_method method = rol::defaults::luma_method): method(method) {}
    luma_method method;
};

constexpr detail::intensity_t intensity;
constexpr detail::lightness_t lightness;
constexpr detail::value_t value;

using greyscale_method = std::variant<luma, detail::intensity_t, detail::lightness_t, detail::value_t>;

} // namespace rol::generic


namespace rol::generic::defaults {

constexpr rol::generic::greyscale_method greyscale_method = rol::generic::luma();

} // namespace rol::generic::defaults
