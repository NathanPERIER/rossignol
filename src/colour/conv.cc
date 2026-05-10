#include "rossignol/colour/conv.hh"

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

double compute_lightness(const min_max_channel& minmax) {
    return (static_cast<double>(minmax.max) + static_cast<double>(minmax.min)) / 2.0;
}

} // anonymous namespace


namespace rol {

double chroma(const rgba& col) {
    return ::compute_chroma(::min_max_channel::from(col));
}

double hue(const rgba& col) {
    const ::min_max_channel minmax = ::min_max_channel::from(col);
    const double chroma = ::compute_chroma(minmax);
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

template <>
double saturation<hsl>(const rgba& col) {
    const ::min_max_channel minmax = ::min_max_channel::from(col);
    return ::compute_chroma(minmax) / (1 - std::abs(2.0 * ::compute_lightness(minmax) / 255.0 - 1.0));
}

template <>
double saturation<hsv>(const rgba& col) {
    const ::min_max_channel minmax = ::min_max_channel::from(col);
    return ::compute_chroma(minmax) / (static_cast<double>(minmax.max) / 255.0);
}

template <>
double saturation<hsi>(const rgba& col) {
    const double intensity = intensity_coeff(col);
    if(intensity == 0.0) {
        return 0.0;
    }
    return 1 - (static_cast<double>(std::min(std::min(col.r, col.g), col.b)) / intensity);
}

uint8_t lightness(const rgba& col) {
    return static_cast<uint8_t>(::compute_lightness(::min_max_channel::from(col)));
}

double lightness_coeff(const rgba& col) {
    return ::compute_lightness(::min_max_channel::from(col)) / 255.0;
}

} // namespace rol