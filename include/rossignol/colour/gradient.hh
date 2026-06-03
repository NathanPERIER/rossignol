
#pragma once

#include <optional>
#include <vector>

#include "rossignol/colour/greyscale.hh"
#include "rossignol/colour/rgb.hh"
#include "rossignol/math/interpolation.hh"


namespace rol {

// TODO: interpolation method (srgb, srgb-linear, lab, oklab, xyz, hsl, hwb, lch, oklch, ...)
// TODO: hue interpolation method (shorter, longer, increasing, decreasing)

class gradient;

} // namespace rol


namespace rol::detail {

class gradient_builder {
public:
    gradient_builder(const rgba& col):_last_explicit_weight_idx(0) {
        _colour_points.push_back(colour_opt_weight { .col = col, .weight = 0.0 });
    }

    gradient_builder& add(const rgba& col) {
        _colour_points.push_back(colour_opt_weight { .col = col });
        return *this;
    }

    gradient_builder& add(const rgba& col, double weight) {
        double prev_weight = _colour_points[_last_explicit_weight_idx].weight.value();
        if(weight <= prev_weight) {
            throw std::runtime_error("Weights in a gradient must always increase");
        }
        double weight_increase = (weight - prev_weight) / (_colour_points.size() - _last_explicit_weight_idx);
        // Now we can compute the previously implicit weights before the current colour
        for(std::size_t i = _last_explicit_weight_idx + 1; i < _colour_points.size(); i++) {
            prev_weight += weight_increase;
            _colour_points[_last_explicit_weight_idx].weight = prev_weight;
        }
        _colour_points.push_back(colour_opt_weight { .col = col, .weight = weight });
        _last_explicit_weight_idx = _colour_points.size() - 1;
        return *this;
    }

    gradient complete(const rgba& col);

private:
    struct colour_opt_weight {
        rgba col;
        std::optional<double> weight = std::nullopt;
    };
    std::vector<colour_opt_weight> _colour_points;
    std::size_t _last_explicit_weight_idx;
};

} // namespace rol::detail


namespace rol {

rgba colour_mix(const rgba& col1, double weight1, const rgba& col2);

class gradient {
private:
    struct weighted_colour {
        rgba col;
        double weight;
    };
    std::array<rgba, 256> _colours;
    std::vector<weighted_colour> _colour_points;
};

} // namespace rol
