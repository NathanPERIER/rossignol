
#include "rossignol/algorithm/generators/generate_image.hh"

#include "rossignol/generic/pipeline.hh"

#include "rossignol/generic/filters.hh"
#include "rossignol/generic/greyscale.hh"
#include "rossignol/generic/sinks/file.hh"


int main() {
    const rol::coefficient_plane base = rol::algo::generate_image<double>(rol::math::vec2u{ .x = 255, .y = 120 }, [](std::size_t x, std::size_t) -> double {
        return static_cast<double>(x) / 255.0;
    });

    (base | rol::generic::low_pass(0.5, rol::generic::unit_step) | rol::generic::to_greyscale()) > rol::generic::file_sink("filters_low_pass_unit.png");
    (base | rol::generic::low_pass(0.5, rol::generic::sigmoid{}) | rol::generic::to_greyscale()) > rol::generic::file_sink("filters_low_pass_sigmoid.png");
    (base | rol::generic::low_pass(0.5, rol::generic::sigmoid{ .steepness = 0.1 }) | rol::generic::to_greyscale()) > rol::generic::file_sink("filters_low_pass_sigmoid_wide.png");
    (base | rol::generic::low_pass(0.5, rol::generic::sigmoid{ .steepness = 1000.0 }) | rol::generic::to_greyscale()) > rol::generic::file_sink("filters_low_pass_sigmoid_narrow.png");

    (base | rol::generic::high_pass(0.5, rol::generic::unit_step) | rol::generic::to_greyscale()) > rol::generic::file_sink("filters_high_pass_unit.png");
    (base | rol::generic::high_pass(0.5, rol::generic::sigmoid{}) | rol::generic::to_greyscale()) > rol::generic::file_sink("filters_high_pass_sigmoid.png");
    (base | rol::generic::high_pass(0.5, rol::generic::sigmoid{ .steepness = 0.1 }) | rol::generic::to_greyscale()) > rol::generic::file_sink("filters_high_pass_sigmoid_wide.png");
    (base | rol::generic::high_pass(0.5, rol::generic::sigmoid{ .steepness = 1000.0 }) | rol::generic::to_greyscale()) > rol::generic::file_sink("filters_high_pass_sigmoid_narrow.png");

    (base | rol::generic::band_pass(0.4, 0.8, rol::generic::unit_step) | rol::generic::to_greyscale()) > rol::generic::file_sink("filters_band_pass_unit.png");
    (base | rol::generic::band_pass(0.4, 0.8, rol::generic::sigmoid{}) | rol::generic::to_greyscale()) > rol::generic::file_sink("filters_band_pass_sigmoid.png");
    (base | rol::generic::band_pass(0.4, 0.8, rol::generic::sigmoid{ .steepness = 0.1 }) | rol::generic::to_greyscale()) > rol::generic::file_sink("filters_band_pass_sigmoid_wide.png");
    (base | rol::generic::band_pass(0.4, 0.8, rol::generic::sigmoid{ .steepness = 1000.0 }) | rol::generic::to_greyscale()) > rol::generic::file_sink("filters_band_pass_sigmoid_narrow.png");

    (base | rol::generic::band_pass(0.8, 0.4, rol::generic::unit_step) | rol::generic::to_greyscale()) > rol::generic::file_sink("filters_band_stop_unit.png");
    (base | rol::generic::band_pass(0.8, 0.4, rol::generic::sigmoid{}) | rol::generic::to_greyscale()) > rol::generic::file_sink("filters_band_stop_sigmoid.png");
    (base | rol::generic::band_pass(0.8, 0.4, rol::generic::sigmoid{ .steepness = 0.1 }) | rol::generic::to_greyscale()) > rol::generic::file_sink("filters_band_stop_sigmoid_wide.png");
    (base | rol::generic::band_pass(0.8, 0.4, rol::generic::sigmoid{ .steepness = 1000.0 }) | rol::generic::to_greyscale()) > rol::generic::file_sink("filters_band_stop_sigmoid_narrow.png");

    return 0;
}
