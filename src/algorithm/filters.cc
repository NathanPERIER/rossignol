#include "rossignol/algorithm/filters.hh"

#include "rossignol/algorithm/map_pixels.hh"
#include "rossignol/math/arithmetic/logistic_function.hh"


namespace {

constexpr double sigmoid_steepness_factor = 128.0;

} // anonymous namespace


namespace rol::algo {

/*--------------------+
|  Unit step          |
+--------------------*/

void high_pass_unit_step_inplace(coefficient_plane& img, double threshold) {
    edit_pixels_inplace(img, [&threshold](double& pixel) { if(pixel < threshold) { pixel = 0.0; } });
}

void low_pass_unit_step_inplace(coefficient_plane& img, double threshold) {
    edit_pixels_inplace(img, [&threshold](double& pixel) { if(pixel > threshold) { pixel = 0.0; } });
}

void band_pass_unit_step_inplace(coefficient_plane& img, double low_threshold, double high_threshold) {
    if(high_threshold < low_threshold) {
        // When the low threshold is the greatest, this becomes a band-stop filter
        edit_pixels_inplace(img, [&low_threshold, &high_threshold](double& pixel) {
            if(high_threshold < pixel && pixel < low_threshold) {
                pixel = 0.0;
            }
        });
        return;
    }
    edit_pixels_inplace(img, [&low_threshold, &high_threshold](double& pixel) {
        if(pixel < low_threshold || high_threshold < pixel) {
            pixel = 0.0;
        }
    });
}


/*--------------------+
|  Sigmoid            |
+--------------------*/

void high_pass_sigmoid_inplace(coefficient_plane& img, double threshold, double steepness) {
    math::logistic_function f(threshold, steepness * ::sigmoid_steepness_factor);
    edit_pixels_inplace(img, [&f](double& pixel) { pixel *= f(pixel); });
}

void low_pass_sigmoid_inplace(coefficient_plane& img, double threshold, double steepness) {
    math::logistic_function f(threshold, steepness * ::sigmoid_steepness_factor);
    edit_pixels_inplace(img, [&f](double& pixel) { pixel *= (1.0 - f(pixel)); });
}

void band_pass_sigmoid_inplace(coefficient_plane& img, double low_threshold, double high_threshold, double steepness) {
    const math::logistic_function f_low(low_threshold, steepness * ::sigmoid_steepness_factor);
    const math::logistic_function f_high(high_threshold, steepness * ::sigmoid_steepness_factor);

    if(low_threshold <= high_threshold) {
        const auto band_pass = [&f_high, &f_low](const double& x) -> double {
            return f_low(x) * (1.0 - f_high(x));
        };
        edit_pixels_inplace(img, [&band_pass](double& pixel) { pixel *= band_pass(pixel); });
    } else {
        // When the low threshold is the greatest, this becomes a band-stop filter
        const auto band_stop = [&f_high, &f_low](const double& x) -> double {
            return 1.0 - f_high(x) * (1.0 - f_low(x));
        };
        edit_pixels_inplace(img, [&band_stop](double& pixel) { pixel *= band_stop(pixel); });
    }
}

} // namespace rol::algo
