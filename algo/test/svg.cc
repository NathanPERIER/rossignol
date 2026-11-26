
#include <unordered_map>

#include <catch2/catch_test_macros.hpp>

#include <rossignol/algorithm/svg.hh>
#include <rossignol/algorithm/generators/template.hh>


using namespace rol::literals;

namespace {

constexpr rol::rgba red = "#ff0000"_rgba;
constexpr rol::rgba green = "#00ff00"_rgba;
constexpr rol::rgba blue = "#0000ff"_rgba;

const std::unordered_map<char, rol::rgba> template_colours {
    { ' ', "#00000000"_rgba },
    { 'r', red },
    { 'g', green },
    { 'b', blue }
};

} // anonymous namespace


namespace rol::algo {

std::ostream& operator<<(std::ostream& out, const rol::algo::svg_tile& tile) {
    out << fmt::to_string(tile.fill_colour);
    for(const rol::math::vec2u& point: tile.outline) {
        out << " (" << point.x << ',' << point.y << ')';
    }
    for(const rol::algo::svg_tile::path& hole: tile.holes) {
        out << " -";
        for(const rol::math::vec2u& point: hole) {
            out << " (" << point.x << ',' << point.y << ')';
        }
    }
    return out;
}

} // namespace rol::algo


TEST_CASE("Basic shapes") {
    const std::vector<std::string> template_pixels {
        "         ",
        " r ggbb  ",
        "   gg bb ",
        "         ",
        "         "
    };
    const rol::rgb_image img = rol::algo::make_from_template(template_pixels, ::template_colours);

    const std::vector<rol::algo::svg_tile> tiles = rol::algo::get_svg_tiles(img);
    REQUIRE(tiles.size() == 3);
    CHECK(tiles[0].is_rectangle());
    CHECK(tiles[0] == rol::algo::svg_tile {
        .fill_colour = ::red,
        .outline = {
            rol::math::vec2u{ .x = 1, .y = 1 },
            rol::math::vec2u{ .x = 1, .y = 2 },
            rol::math::vec2u{ .x = 2, .y = 2 },
            rol::math::vec2u{ .x = 2, .y = 1 }
        }
    });
    CHECK(tiles[1].is_rectangle());
    CHECK(tiles[1] == rol::algo::svg_tile {
        .fill_colour = ::green,
        .outline = {
            rol::math::vec2u{ .x = 3, .y = 1 },
            rol::math::vec2u{ .x = 3, .y = 3 },
            rol::math::vec2u{ .x = 5, .y = 3 },
            rol::math::vec2u{ .x = 5, .y = 1 }
        }
    });
    CHECK(!tiles[2].is_rectangle());
    CHECK(tiles[2] == rol::algo::svg_tile {
        .fill_colour = ::blue,
        .outline = {
            rol::math::vec2u{ .x = 5, .y = 1 },
            rol::math::vec2u{ .x = 5, .y = 2 },
            rol::math::vec2u{ .x = 6, .y = 2 },
            rol::math::vec2u{ .x = 6, .y = 3 },
            rol::math::vec2u{ .x = 8, .y = 3 },
            rol::math::vec2u{ .x = 8, .y = 2 },
            rol::math::vec2u{ .x = 7, .y = 2 },
            rol::math::vec2u{ .x = 7, .y = 1 }
        }
    });
}


TEST_CASE("Merge two connected components") {
    const std::vector<std::string> template_pixels {
        "               ",
        "   rrr   rrr   ",
        "  rrrrr rrrrr  ",
        " rrrrrrrrrrrrr ",
        " rrrrrrrrrrrrr ",
        " rrrrrrrrrrrrr ",
        "  rrrrrrrrrrr  ",
        "   rrrrrrrrr   ",
        "    rrrrrrr    ",
        "     rrrrr     ",
        "      rrr      ",
        "       r       ",
        "               "
    };
    const rol::rgb_image img = rol::algo::make_from_template(template_pixels, ::template_colours);

    const std::vector<rol::algo::svg_tile> tiles = rol::algo::get_svg_tiles(img);
    REQUIRE(tiles.size() == 1);
    CHECK(tiles[0] == rol::algo::svg_tile {
        .fill_colour = ::red,
        .outline = {
            rol::math::vec2u { .x =  3, .y =  1 },
            rol::math::vec2u { .x =  3, .y =  2 },
            rol::math::vec2u { .x =  2, .y =  2 },
            rol::math::vec2u { .x =  2, .y =  3 },
            rol::math::vec2u { .x =  1, .y =  3 },
            rol::math::vec2u { .x =  1, .y =  6 },
            rol::math::vec2u { .x =  2, .y =  6 },
            rol::math::vec2u { .x =  2, .y =  7 },
            rol::math::vec2u { .x =  3, .y =  7 },
            rol::math::vec2u { .x =  3, .y =  8 },
            rol::math::vec2u { .x =  4, .y =  8 },
            rol::math::vec2u { .x =  4, .y =  9 },
            rol::math::vec2u { .x =  5, .y =  9 },
            rol::math::vec2u { .x =  5, .y = 10 },
            rol::math::vec2u { .x =  6, .y = 10 },
            rol::math::vec2u { .x =  6, .y = 11 },
            rol::math::vec2u { .x =  7, .y = 11 },
            rol::math::vec2u { .x =  7, .y = 12 },
            rol::math::vec2u { .x =  8, .y = 12 },
            rol::math::vec2u { .x =  8, .y = 11 },
            rol::math::vec2u { .x =  9, .y = 11 },
            rol::math::vec2u { .x =  9, .y = 10 },
            rol::math::vec2u { .x = 10, .y = 10 },
            rol::math::vec2u { .x = 10, .y =  9 },
            rol::math::vec2u { .x = 11, .y =  9 },
            rol::math::vec2u { .x = 11, .y =  8 },
            rol::math::vec2u { .x = 12, .y =  8 },
            rol::math::vec2u { .x = 12, .y =  7 },
            rol::math::vec2u { .x = 13, .y =  7 },
            rol::math::vec2u { .x = 13, .y =  6 },
            rol::math::vec2u { .x = 14, .y =  6 },
            rol::math::vec2u { .x = 14, .y =  3 },
            rol::math::vec2u { .x = 13, .y =  3 },
            rol::math::vec2u { .x = 13, .y =  2 },
            rol::math::vec2u { .x = 12, .y =  2 },
            rol::math::vec2u { .x = 12, .y =  1 },
            rol::math::vec2u { .x =  9, .y =  1 },
            rol::math::vec2u { .x =  9, .y =  2 },
            rol::math::vec2u { .x =  8, .y =  2 },
            rol::math::vec2u { .x =  8, .y =  3 },
            rol::math::vec2u { .x =  7, .y =  3 },
            rol::math::vec2u { .x =  7, .y =  2 },
            rol::math::vec2u { .x =  6, .y =  2 },
            rol::math::vec2u { .x =  6, .y =  1 }
        }
    });
}


TEST_CASE("Shapes with holes") {
    const std::vector<std::string> template_pixels {
        " rrrrrrrrrrrrrrrrrr ",
        " r     rrrbbbbbbr   ",
        " r    rrrrb    br   ",
        " r   rr  rb    br   ",
        " r  rr   rbbbbbbr   ",
        " rrrrrrrrrrrrrrrrr  ",
    };
    const rol::rgb_image img = rol::algo::make_from_template(template_pixels, ::template_colours);

    const std::vector<rol::algo::svg_tile> tiles = rol::algo::get_svg_tiles(img);
    REQUIRE(tiles.size() == 2);
    CHECK(tiles[0] == rol::algo::svg_tile {
        .fill_colour = ::red,
        .outline = {
            rol::math::vec2u{ .x =  1, .y = 0 },
            rol::math::vec2u{ .x =  1, .y = 6 },
            rol::math::vec2u{ .x = 18, .y = 6 },
            rol::math::vec2u{ .x = 18, .y = 5 },
            rol::math::vec2u{ .x = 17, .y = 5 },
            rol::math::vec2u{ .x = 17, .y = 1 },
            rol::math::vec2u{ .x = 19, .y = 1 },
            rol::math::vec2u{ .x = 19, .y = 0 }
        },
        .holes = {
            {
                rol::math::vec2u{ .x = 10, .y = 5 },
                rol::math::vec2u{ .x = 10, .y = 1 },
                rol::math::vec2u{ .x = 16, .y = 1 },
                rol::math::vec2u{ .x = 16, .y = 5 }
            },
            {
                rol::math::vec2u{ .x = 6, .y = 5 },
                rol::math::vec2u{ .x = 6, .y = 4 },
                rol::math::vec2u{ .x = 7, .y = 4 },
                rol::math::vec2u{ .x = 7, .y = 3 },
                rol::math::vec2u{ .x = 9, .y = 3 },
                rol::math::vec2u{ .x = 9, .y = 5 }
            },
            {
                rol::math::vec2u{ .x = 2, .y = 5 },
                rol::math::vec2u{ .x = 2, .y = 1 },
                rol::math::vec2u{ .x = 7, .y = 1 },
                rol::math::vec2u{ .x = 7, .y = 2 },
                rol::math::vec2u{ .x = 6, .y = 2 },
                rol::math::vec2u{ .x = 6, .y = 3 },
                rol::math::vec2u{ .x = 5, .y = 3 },
                rol::math::vec2u{ .x = 5, .y = 4 },
                rol::math::vec2u{ .x = 4, .y = 4 },
                rol::math::vec2u{ .x = 4, .y = 5 }
            }
        }
    });
    CHECK(!tiles[1].is_rectangle());
    CHECK(tiles[1] == rol::algo::svg_tile {
        .fill_colour = ::blue,
        .outline = {
            rol::math::vec2u{ .x = 10, .y = 1 },
            rol::math::vec2u{ .x = 10, .y = 5 },
            rol::math::vec2u{ .x = 16, .y = 5 },
            rol::math::vec2u{ .x = 16, .y = 1 }
        },
        .holes = {
            {
                rol::math::vec2u{ .x = 11, .y = 4 },
                rol::math::vec2u{ .x = 11, .y = 2 },
                rol::math::vec2u{ .x = 15, .y = 2 },
                rol::math::vec2u{ .x = 15, .y = 4 }
            }
        }
    });
}
