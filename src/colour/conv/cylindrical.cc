#include "rossignol/colour/conv/cylindrical.hh"

#include <algorithm>
#include <cmath>


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

} // anonymous namespace


namespace rol {

double chroma(const rgba& col) {
    return ::compute_chroma(::min_max_channel::from(col));
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

} // namespace rol
