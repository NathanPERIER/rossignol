#include "rossignol/colour/conv/cylindrical.hh"

#include <algorithm>
#include <cmath>

#include "rossignol/utils/typing/functional.hh"


namespace {

struct min_max_channel {
    const uint8_t min;
    const uint8_t max;

    static min_max_channel from(const rol::rgba& col) {
        if(col.r < col.g) {
            if(col.b <= col.r) {
                return min_max_channel { .min = col.b, .max = col.g };
            }
            if(col.g <= col.b) {
                return min_max_channel { .min = col.r, .max = col.b };
            }
            return min_max_channel { .min = col.r, .max = col.g };
        } else { // col.g <= col.r
            if(col.b <= col.g) {
                return min_max_channel { .min = col.b, .max = col.r };
            }
            if(col.r <= col.b) {
                return min_max_channel { .min = col.g, .max = col.b };
            }
            return min_max_channel { .min = col.g, .max = col.r };
        }
    }
};

double compute_chroma(const min_max_channel& minmax) {
    return static_cast<double>(minmax.max - minmax.min) / 255.0;
}

double compute_hue(const rol::rgba& col, const min_max_channel& minmax, const double& chroma) {
    double res = 0;
    if(chroma != 0) {
        if(minmax.max == col.r) {
            res = std::fmod(static_cast<double>(col.g - col.b) / chroma, 6);
        } else if(minmax.max == col.g) {
            res = (static_cast<double>(col.b - col.r) / chroma) + 2.0;
        } else {
            res = (static_cast<double>(col.r - col.g) / chroma) + 4.0;
        }
    }
    return (res * 60.0) / 360.0;
}

double compute_hsl_saturation(const double& chroma, const double& lightness) {
    return chroma / (1 - std::abs(2.0 * lightness / 255.0 - 1.0));
}

double compute_hsv_saturation(const min_max_channel& minmax, const double& chroma) {
    return chroma / (static_cast<double>(minmax.max) / 255.0);
}

double compute_hsi_saturation(const double& intensity, const uint8_t& min_channel) {
    if(intensity == 0.0) {
        return 0.0;
    }
    return 1.0 - ((static_cast<double>(min_channel) / 255.0) / intensity);
}

double compute_lightness(const min_max_channel& minmax) {
    return (static_cast<double>(minmax.max) + static_cast<double>(minmax.min)) / 2.0;
}


/// @brief helper function for converting cylindrical colours to RGB
/// @tparam InputColour cylindrical colour type
/// @tparam OffsetFunc function to compute the final offset
template <typename InputColour, typename OffsetFunc>
requires(rol::yield_invocable<OffsetFunc, double, const InputColour&, const double&>)
rol::rgba cylindrical_to_rgb(const InputColour& col, const OffsetFunc& compute_offset) {
    // The chroma is also the offset of the largest component
    const double chroma = rol::chroma(col);
    // Normally this is H' = H / 60, with H in [0, 360[
    // But our h is already H / 360, hence we only need to multiply by 6 to get H'
    const double hue_div60 = rol::hue_coeff(col) * 6.0;
    // Offset of the second largest component
    const double second_component = chroma * (1.0 - std::abs(std::fmod(hue_div60, 2.0) - 1.0));

    // Compute the final offset, which is a specific operation for each colour type
    const double final_offset = compute_offset(col, chroma);

    // Compute the RGB channel values
    double r;
    double g;
    double b;
    if(hue_div60 < 1) {
        r = chroma;
        g = second_component;
        b = 0;
    } else if(hue_div60 < 2) {
        r = second_component;
        g = chroma;
        b = 0;
    } else if(hue_div60 < 3) {
        r = 0;
        g = chroma;
        b = second_component;
    } else if(hue_div60 < 4) {
        r = 0;
        g = second_component;
        b = chroma;
    } else if(hue_div60 < 6) {
        r = second_component;
        g = 0;
        b = chroma;
    } else {
        r = chroma;
        g = 0;
        b = second_component;
    }
    r += final_offset;
    g += final_offset;
    b += final_offset;
    if constexpr (std::same_as<InputColour, rol::hsi>) {
        if(r > 1.0 || g > 1.0 || b > 1.0) {
            const double max_channel = std::max(std::max(r, g), b);
            r /= max_channel;
            g /= max_channel;
            b /= max_channel;
        }
    }
    return rol::rgba {
        .r = static_cast<uint8_t>(r * 255.0),
        .g = static_cast<uint8_t>(g * 255.0),
        .b = static_cast<uint8_t>(b * 255.0),
        .a = 255
    };
}

} // anonymous namespace


namespace rol {

double chroma(const rgba& col) {
    return ::compute_chroma(::min_max_channel::from(col));
}

double chroma(const hsi& col) {
    return (3.0 * col.i * col.s) / (2.0 - std::abs(std::fmod(col.h * 6.0, 2.0) - 1.0));
}

double chroma(const hsl& col) {
    return (1.0 - std::abs(2.0 * col.l - 1.0)) * col.s;
}

double chroma(const hsv& col) {
    return col.v * col.s;
}

double hue_coeff(const rgba& col) {
    const ::min_max_channel minmax = ::min_max_channel::from(col);
    const double chroma = ::compute_chroma(minmax);
    return ::compute_hue(col, minmax, chroma);
}

template <>
double saturation<hsl>(const rgba& col) {
    const ::min_max_channel minmax = ::min_max_channel::from(col);
    return ::compute_hsl_saturation(::compute_chroma(minmax), ::compute_lightness(minmax));
}

template <>
double saturation<hsv>(const rgba& col) {
    const ::min_max_channel minmax = ::min_max_channel::from(col);
    return ::compute_hsv_saturation(minmax, ::compute_chroma(minmax));
}

template <>
double saturation<hsi>(const rgba& col) {
    return ::compute_hsi_saturation(intensity_coeff(col), std::min(std::min(col.r, col.g), col.b));
}

uint8_t lightness(const rgba& col) {
    return static_cast<uint8_t>(::compute_lightness(::min_max_channel::from(col)));
}

double lightness_coeff(const rgba& col) {
    return ::compute_lightness(::min_max_channel::from(col)) / 255.0;
}


/*------------------------------+
|  RGB to cylindrical           |
+------------------------------*/

template <>
hsi colour_conv<hsi>(const rgba& col) {
    const ::min_max_channel minmax = ::min_max_channel::from(col);
    const double intensity = intensity_coeff(col);
    return hsi {
        .h = ::compute_hue(col, minmax, ::compute_chroma(minmax)),
        .s = ::compute_hsi_saturation(intensity, minmax.min),
        .i = intensity
    };
}

template <>
hsl colour_conv<hsl>(const rgba& col) {
    const ::min_max_channel minmax = ::min_max_channel::from(col);
    const double chroma = ::compute_chroma(minmax);
    const double lightness = ::compute_lightness(minmax);
    return hsl {
        .h = ::compute_hue(col, minmax, chroma),
        .s = ::compute_hsl_saturation(chroma, lightness),
        .l = lightness
    };
}

template <>
hsv colour_conv<hsv>(const rgba& col) {
    const ::min_max_channel minmax = ::min_max_channel::from(col);
    const double chroma = ::compute_chroma(minmax);
    return hsv {
        .h = ::compute_hue(col, minmax, chroma),
        .s = ::compute_hsv_saturation(minmax, chroma),
        .v = static_cast<double>(minmax.max) / 255.0
    };
}


/*------------------------------+
|  Cylindrical to greyscale     |
+------------------------------*/

template <>
greyscalea colour_conv<greyscalea>(const hsi& col) {
    return {
        .grey = static_cast<uint8_t>(col.i * 255.0),
        .a = 255
    };
}

template <>
greyscalea colour_conv<greyscalea>(const hsl& col) {
    return {
        .grey = static_cast<uint8_t>(col.l * 255.0),
        .a = 255
    };
}

template <>
greyscalea colour_conv<greyscalea>(const hsv& col) {
    return {
        .grey = static_cast<uint8_t>(col.v * 255.0),
        .a = 255
    };
}


/*------------------------------+
|  Cylindrical to RGB           |
+------------------------------*/

template <>
rgba colour_conv<rgba>(const hsi& col) {
    return ::cylindrical_to_rgb(col, [](const hsi& col, const double&) {
        return col.i * (1 - col.s);
    });
}

template <>
rgba colour_conv<rgba>(const hsl& col) {
    return ::cylindrical_to_rgb(col, [](const hsl& col, const double& chroma) {
        return col.l - chroma / 2.0;
    });
}

template <>
rgba colour_conv<rgba>(const hsv& col) {
    return ::cylindrical_to_rgb(col, [](const hsv& col, const double& chroma) {
        return col.v - chroma;
    });
}


/*------------------------------+
|  Cylindrical to cylindrical   |
+------------------------------*/

template <>
hsv colour_conv<hsv>(const hsl& col) {
    double v = col.l + col.s * std::min(col.l, 1.0 - col.l);
    return hsv {
        .h = col.h,
        .s = (v > 0.0) ? (2.0 * (1.0 - col.l / v)) : 0.0,
        .v = v
    };
}

template <>
hsl colour_conv<hsl>(const hsv& col) {
    double l = col.v * (1.0 - col.s / 2.0);
    return hsl {
        .h = col.h,
        .s = (l > 0.0 && l < 1.0) ? (col.v - l) / std::min(l, 1 - l) : 0.0,
        .l = l
    };
}

} // namespace rol
