
#pragma once

namespace rol::generic {

enum class prepare_colour_optims {
    none,
    greyscale
};

} // namespace rol::generic


namespace rol::generic::defaults {

constexpr rol::generic::prepare_colour_optims prepare_colour_optims = rol::generic::prepare_colour_optims::none;

} // namespace rol::generic::defaults
