
#pragma once

#include "rossignol/image/image.hh"
#include "rossignol/utils/typing/functional.hh"


namespace rol::algo {

template <typename Colour, typename Generator>
requires(yield_invocable<Generator, Colour> || yield_invocable<Generator, Colour, std::size_t, std::size_t>)
basic_image<Colour> generate_image(const math::vec2u& size, Generator& gen) {
    basic_image<Colour> res(size);

    for(std::size_t y = 0; y < res.height(); y++) {
        std::span<Colour> row = res[y];
        for(std::size_t x = 0; x < res.width(); x++) {
            if constexpr (std::invocable<Generator, std::size_t, std::size_t>) {
                row[x] = gen(x, y);
            } else {
                row[x] = gen();
            }
        }
    }

    return res;
}

template <typename Colour, typename Generator>
requires((yield_invocable<Generator, Colour> || yield_invocable<Generator, Colour, std::size_t, std::size_t>))
basic_image<Colour> generate_image(const math::vec2u& size, Generator&& gen_lvalue) {
    Generator gen = std::move(gen_lvalue);
    return generate_image<Colour, Generator>(size, gen);
}

} // namespace rol::algo
