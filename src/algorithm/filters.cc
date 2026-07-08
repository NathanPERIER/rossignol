#include "rossignol/algorithm/filters.hh"

#include "rossignol/algorithm/map_pixels.hh"


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

} // namespace rol::algo
