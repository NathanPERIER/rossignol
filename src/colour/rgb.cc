#include "rossignol/colour/rgb.hh"

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

double rgba::chroma() const {
    return ::compute_chroma(::min_max_channel::from(*this));
}

double rgba::hue() const {
    const ::min_max_channel minmax = ::min_max_channel::from(*this);
    const double chroma = ::compute_chroma(minmax);
    double res = 0;
    if(chroma != 0) {
        if(minmax.max == r) {
            res = std::fmod(static_cast<double>(g - b) / chroma, 6);
        } else if(minmax.max == g) {
            res = (static_cast<double>(b - r) / chroma) + 2.0;
        } else {
            res = (static_cast<double>(r - g) / chroma) + 4.0;
        }
    }
    return res * 60.0;
}

double rgba::hsl_saturation() const {
    const ::min_max_channel minmax = ::min_max_channel::from(*this);
    return ::compute_chroma(minmax) / (1 - std::abs(2.0 * ::compute_lightness(minmax) / 255.0 - 1.0));
}

double rgba::hsv_saturation() const {
    const ::min_max_channel minmax = ::min_max_channel::from(*this);
    return ::compute_chroma(minmax) / (static_cast<double>(minmax.max) / 255.0);
}

double rgba::hsi_saturation() const {
    const double intensity = intensity_coeff();
    if(intensity == 0) {
        return 0.0;
    }
    return 1 - (static_cast<double>(std::min(std::min(r, g), b)) / intensity);
}

uint8_t rgba::lightness() const {
    return static_cast<uint8_t>(::compute_lightness(::min_max_channel::from(*this)));
}

uint8_t rgba::value() const {
    return std::max(std::max(r, g), b);
}

uint8_t rgba::intensity() const {
    return static_cast<uint8_t>(
        (static_cast<uint16_t>(r) + static_cast<uint16_t>(g) + static_cast<uint16_t>(b)) / 3
    );
};

double rgba::lightness_coeff() const {
    return ::compute_lightness(::min_max_channel::from(*this)) / 255.0;
}

double rgba::value_coeff() const {
    return static_cast<double>(value()) / 255.0;
}

double rgba::intensity_coeff() const {
    return (static_cast<double>(r) + static_cast<double>(g) + static_cast<double>(b)) / 3.0;
};

} // namespace rol