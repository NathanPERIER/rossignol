
#include <rossignol/algorithm/generators/perlin_noise.hh>

#include <rossignol/generic/pipeline.hh>

#include <rossignol/generic/greyscale.hh>
#include <rossignol/generic/colourmap.hh>

#include <rossignol/image/io.hh>
#include <rossignol/utils/io/file.hh>


int main() {
    const rol::coefficient_plane noise = rol::algo::perlin_noise(rol::math::vec2u { .x = 300, .y = 180 }, 30);

    rol::greyscale_image noise_greyscale = noise | rol::generic::to_greyscale();
    // TODO add support for image sinks
    rol::write_to_file("perlin.png", rol::dump_image(noise_greyscale));

    // TODO add constructor that just takes a rol::builtin_colourmap
    rol::rgb_image noise_coloured = noise | rol::generic::apply_colourmap(rol::get_colourmap<rol::builtin_colourmap::twilight>());
    rol::write_to_file("perlin_coloured.png", rol::dump_image(noise_coloured));

    rol::greyscale_image noise_fine = rol::algo::perlin_noise(rol::math::vec2u { .x = 300, .y = 180 }, 10) | rol::generic::to_greyscale();
    rol::write_to_file("perlin_fine.png", rol::dump_image(noise_fine));

    rol::greyscale_image noise_coarse = rol::algo::perlin_noise(rol::math::vec2u { .x = 300, .y = 180 }, 90) | rol::generic::to_greyscale();
    rol::write_to_file("perlin_coarse.png", rol::dump_image(noise_coarse));

    return 0;
}
