
#pragma once

#include <unordered_map>

#include "rossignol/image/image.hh"


namespace rol::algo {

template <typename Colour>
basic_image<Colour> make_from_template(std::span<const std::string> pixels, const std::unordered_map<char, Colour>& colours) {
    ROL_CHECK(pixels.size() > 0, "An image cannot be empty (at least one line is required)");
    const math::vec2u size { .x = pixels[0].size(), .y = pixels.size() };
    ROL_CHECK(size.x > 0, "An image cannot be empty (each line must contain at least one pixel)");
    basic_image<Colour> res(size);
    for(std::size_t y = 0; y < res.height(); y++) {
        std::string_view in_line = pixels[y];
        std::span<Colour> out_line = res[y];
        ROL_CHECK(in_line.size() == out_line.size(), "All input lines must have the same size (expected={}, found={})", out_line.size(), in_line.size());
        for(std::size_t x = 0; x < out_line.size(); x++) {
            const auto it = colours.find(in_line[x]);
            ROL_CHECK(it != colours.end(), "All characters must be mapped to a colour");
            out_line[x] = it->second;
        }
    }
    return res;
}

} // namespace rol::algo
