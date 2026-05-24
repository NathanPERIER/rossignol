
#pragma once

#include <array>
#include <cstdint>
#include <memory>

#include <rossignol/colour/rgb.hh>
#include <rossignol/colour/srgb.hh>


namespace rol {

struct colourmap {
    std::array<rgba, 256> colours;
    std::array<srgb, 256> coefficients;

    /// @brief retrieves the colour associated with an exact index
    /// @param idx the index of the colour in the list
    /// @note this is very fast because the colours are pre-computed, but with limited resolution
    rgba operator[](uint8_t idx) const { return colours[idx]; }

    /// @brief computes a colour for a given floating-point coefficient
    /// @param coeff the coefficient for which we want to compute a colour (between 0.0 and 1.0)
    /// @note this can potentially have finer resolution but it is more expensive to compute
    rgba operator()(double coeff) const {
        if(coeff >= 1.0) {
            return colours[255];
        }
        const double coeff_scaled = coeff * 255.0;
        // Index of the point that is directly before the required coefficient
        const uint8_t idx = static_cast<uint8_t>(coeff_scaled);
        // The weight to apply on the colour at idx
        const double weight_idx = coeff_scaled - static_cast<double>(idx);
        // The weight to apply on the next colour (directly after the coefficient)
        const double weight_next_idx = 1.0 - weight_idx;
        return rgba {
            .r = static_cast<uint8_t>((coefficients[idx].r * weight_idx + coefficients[idx+1].r * weight_next_idx) * 255.0),
            .g = static_cast<uint8_t>((coefficients[idx].g * weight_idx + coefficients[idx+1].g * weight_next_idx) * 255.0),
            .b = static_cast<uint8_t>((coefficients[idx].b * weight_idx + coefficients[idx+1].b * weight_next_idx) * 255.0),
            .a = 255
        };
    }
};


enum class builtin_colourmap {
    autumn,
    bone,
    cividis,
    cool,
    deepgreen,
    hot,
    hsv,
    inferno,
    jet,
    magma,
    ocean,
    parula,
    pink,
    plasma,
    rainbow,
    spring,
    summer,
    turbo,
    twilight,
    twilight_shifted,
    viridis,
    winter
};


template <builtin_colourmap Colourmap>
std::shared_ptr<const colourmap> get_colourmap();

std::shared_ptr<const colourmap> make_random_colourmap();

} // namespace rol
