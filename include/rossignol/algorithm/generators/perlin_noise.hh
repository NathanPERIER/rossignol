#pragma once

#include <cmath>
#include <random>

#include "rossignol/algorithm/generators/generate_image.hh"


namespace rol::algo::detail {

inline double smoothstep(double x) {
    if(x < 0.0) {
        return 0.0;
    }
    if(x > 1.0) {
        return 1.0;
    }
    return x * x * (3.0 - 2.0 * x);
}

inline double interpolate(double min, double max, double weight) {
    return min + (max - min) * smoothstep(weight);
}

} // namespace rol::algo::detail


namespace rol::algo {

inline coefficient_plane perlin_noise(const math::vec2u& size, std::size_t block_size /*, const math::vec2i& offset */) {
    const math::vec2u grid_size {
        .x = (size.x + block_size - 1) / block_size + 1,
        .y = (size.y + block_size - 1) / block_size + 1
    };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<std::vector<math::vec2f>> edge_gradients;
    edge_gradients.reserve(grid_size.x);
    for(std::size_t x = 0; x < grid_size.x; x++) {
        std::vector<math::vec2f>& row = edge_gradients.emplace_back();
        row.reserve(grid_size.y);
        for(std::size_t x = 0; x < grid_size.x; x++) {
            const double angle = std::generate_canonical<double, std::numeric_limits<uint32_t>::max()>(gen) * 2.0 * std::numbers::pi;
            row.push_back(math::vec2f {
                .x = std::cos(angle),
                .y = std::sin(angle)
            });
        }
    }

    return generate_image<double>(size, [&block_size, grid_size, edge_gradients](std::size_t x, std::size_t y) -> double {
        const math::vec2u current_block {
            .x = x / block_size,
            .y = y / block_size
        };
        const math::vec2f position_in_block {
            .x = static_cast<double>(x - current_block.x * block_size) / static_cast<double>(block_size),
            .y = static_cast<double>(y - current_block.y * block_size) / static_cast<double>(block_size)
        };

        const double top_left = position_in_block * edge_gradients[current_block.x][current_block.y];
        const double top_right = (position_in_block - math::vec2f { .x = 1.0, .y = 0.0 }) * edge_gradients[current_block.x+1][current_block.y];
        const double top = detail::interpolate(top_left, top_right, position_in_block.x);


        const double bottom_left = (position_in_block - math::vec2f { .x = 0.0, .y = 1.0 }) * edge_gradients[current_block.x][current_block.y+1];
        const double bottom_right = (position_in_block - math::vec2f { .x = 1.0, .y = 1.0 }) * edge_gradients[current_block.x+1][current_block.y+1];
        const double bottom = detail::interpolate(bottom_left, bottom_right, position_in_block.x);

        return (1.0 + detail::interpolate(top, bottom, position_in_block.y)) / 2.0;
    });
}

} // namespace rol::algo
