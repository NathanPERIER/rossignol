
#pragma once

#include "rossignol/colour/greyscale.hh"
#include "rossignol/colour/rgb.hh"

/*
 * This file contains builtin colours, taken from the CSS3 specification.
 *
 * The following colours have several names, depending on the entity that introduced them :
 *  - #ff00ff `magenta` (X11) / `fuchsia` (W3C)
 *  - #00ffff `cyan` (X11) / `aqua` (W3C)
 * Both names will be supported, but the X11 name will be preferred.
 *
 * Also, note that unlike `red` (#ff0000) and `blue` (#0000ff), the `green` colour does NOT refer to a
 * pure shade of green in the RGB sense (#00ff00) (that would be `lime`).
 */

namespace rol {

enum class builtin_binary_colour: uint8_t {
    black = 0x00,
    white = 0x01
};

enum class builtin_greyscale_colour: uint16_t {
    dark_grey   = 0xa9ff,
    dim_grey    = 0x69ff,
    gainsboro   = 0xdcff,
    grey        = 0x80ff,
    light_grey  = 0xd3ff,
    silver      = 0xc0ff,
    transparent = 0x0000,
    white_smoke = 0xf5ff
};

enum class builtin_rgb_colour: uint32_t {
    alice_blue             = 0xf0f8ffff,
    antique_white          = 0xfaebd7ff,
    aqua                   = 0x00ffffff,
    aquamarine             = 0x7fffd4ff,
    azure                  = 0xf0ffffff,
    beige                  = 0xf5f5dcff,
    bisque                 = 0xffe4c4ff,
    blanched_almond        = 0xffebcdff,
    blue                   = 0x0000ffff,
    blue_violet            = 0x8a2be2ff,
    brown                  = 0xa52a2aff,
    burly_wood             = 0xdeb887ff,
    cadet_blue             = 0x5f9ea0ff,
    chartreuse             = 0x7fff00ff,
    chocolate              = 0xd2691eff,
    coral                  = 0xff7f50ff,
    cornflower_blue        = 0x6495edff,
    corn_silk              = 0xfff8dcff,
    crimson                = 0xdc143cff,
    cyan                   = 0x00ffffff,
    dark_blue              = 0x00008bff,
    dark_cyan              = 0x008b8bff,
    dark_goldenrod         = 0xb8860bff,
    dark_green             = 0x006400ff,
    dark_khaki             = 0xbdb76bff,
    dark_magenta           = 0x8b008bff,
    dark_olive_green       = 0x556b2fff,
    dark_orange            = 0xff8c00ff,
    dark_orchid            = 0x9932ccff,
    dark_red               = 0x8b0000ff,
    dark_salmon            = 0xe9967aff,
    dark_sea_green         = 0x8fbc8fff,
    dark_slate_blue        = 0x483d8bff,
    dark_slate_grey        = 0x2f4f4fff,
    dark_turquoise         = 0x00ced1ff,
    dark_violet            = 0x9400d3ff,
    deep_pink              = 0xff1493ff,
    deep_sky_blue          = 0x00bfffff,
    dodger_blue            = 0x1e90ffff,
    fire_brick             = 0xb22222ff,
    floral_white           = 0xfffaf0ff,
    forest_green           = 0x228b22ff,
    fuchsia                = 0xff00ffff,
    ghost_white            = 0xf8f8ffff,
    gold                   = 0xffd700ff,
    goldenrod              = 0xdaa520ff,
    green                  = 0x008000ff,
    green_yellow           = 0xadff2fff,
    honeydew               = 0xf0fff0ff,
    hot_pink               = 0xff69b4ff,
    indian_red             = 0xcd5c5cff,
    indigo                 = 0x4b0082ff,
    ivory                  = 0xfffff0ff,
    khaki                  = 0xf0e68cff,
    lavender               = 0xe6e6faff,
    lavender_blush         = 0xfff0f5ff,
    lawn_green             = 0x7cfc00ff,
    lemon_chiffon          = 0xfffacdff,
    light_blue             = 0xadd8e6ff,
    light_coral            = 0xf08080ff,
    light_cyan             = 0xe0ffffff,
    light_goldenrod_yellow = 0xfafad2ff,
    light_green            = 0x90ee90ff,
    light_pink             = 0xffb6c1ff,
    light_salmon           = 0xffa07aff,
    light_sea_green        = 0x20b2aaff,
    light_sky_blue         = 0x87cefaff,
    light_slate_grey       = 0x778899ff,
    light_steel_blue       = 0xb0c4deff,
    light_yellow           = 0xffffe0ff,
    lime                   = 0x00ff00ff,
    lime_green             = 0x32cd32ff,
    linen                  = 0xfaf0e6ff,
    magenta                = 0xff00ffff,
    maroon                 = 0x800000ff,
    medium_aquamarine      = 0x66cdaaff,
    medium_blue            = 0x0000cdff,
    medium_orchid          = 0xba55d3ff,
    medium_purple          = 0x9370dbff,
    medium_sea_green       = 0x3cb371ff,
    medium_slate_blue      = 0x7b68eeff,
    medium_spring_green    = 0x00fa9aff,
    medium_turquoise       = 0x48d1ccff,
    medium_violet_red      = 0xc71585ff,
    midnight_blue          = 0x191970ff,
    mint_cream             = 0xf5fffaff,
    misty_rose             = 0xffe4e1ff,
    moccasin               = 0xffe4b5ff,
    navajo_white           = 0xffdeadff,
    navy                   = 0x000080ff,
    old_lace               = 0xfdf5e6ff,
    olive                  = 0x808000ff,
    olive_drab             = 0x6b8e23ff,
    orange                 = 0xffa500ff,
    orange_red             = 0xff4500ff,
    orchid                 = 0xda70d6ff,
    pale_goldenrod         = 0xeee8aaff,
    pale_green             = 0x98fb98ff,
    pale_turquoise         = 0xafeeeeff,
    pale_violet_red        = 0xdb7093ff,
    papaya_whip            = 0xffefd5ff,
    peach_puff             = 0xffdab9ff,
    peru                   = 0xcd853fff,
    pink                   = 0xffc0cbff,
    plum                   = 0xdda0ddff,
    powder_blue            = 0xb0e0e6ff,
    purple                 = 0x800080ff,
    red                    = 0xff0000ff,
    rosy_brown             = 0xbc8f8fff,
    royal_blue             = 0x4169e1ff,
    saddle_brown           = 0x8b4513ff,
    salmon                 = 0xfa8072ff,
    sandy_brown            = 0xf4a460ff,
    sea_green              = 0x2e8b57ff,
    seashell               = 0xfff5eeff,
    sienna                 = 0xa0522dff,
    sky_blue               = 0x87ceebff,
    slate_blue             = 0x6a5acdff,
    slate_grey             = 0x708090ff,
    snow                   = 0xfffafaff,
    spring_green           = 0x00ff7fff,
    steel_blue             = 0x4682b4ff,
    tan                    = 0xd2b48cff,
    teal                   = 0x008080ff,
    thistle                = 0xd8bfd8ff,
    tomato                 = 0xff6347ff,
    turquoise              = 0x40e0d0ff,
    violet                 = 0xee82eeff,
    wheat                  = 0xf5deb3ff,
    yellow                 = 0xffff00ff,
    yellow_green           = 0x9acd32ff
};

inline bool get_binary_colour(builtin_binary_colour col) {
    return col != builtin_binary_colour::black;
}

inline greyscalea get_greyscale_colour(builtin_greyscale_colour col) {
    return greyscalea {
        .grey = static_cast<uint8_t>((static_cast<uint16_t>(col) & 0xff00) >> 8),
        .a    = static_cast<uint8_t>( static_cast<uint16_t>(col) & 0x00ff      )
    };
}

inline greyscalea get_greyscale_colour(builtin_binary_colour col) {
    return greyscalea {
        .grey = static_cast<uint8_t>((col != builtin_binary_colour::black) ? 255 : 0),
        .a = 255
    };
}

inline rgba get_rgb_colour(builtin_rgb_colour col) {
    return rgba {
        .r = static_cast<uint8_t>((static_cast<uint32_t>(col) & 0xff000000) >> 24),
        .g = static_cast<uint8_t>((static_cast<uint32_t>(col) & 0x00ff0000) >> 16),
        .b = static_cast<uint8_t>((static_cast<uint32_t>(col) & 0x0000ff00) >>  8),
        .a = static_cast<uint8_t>( static_cast<uint32_t>(col) & 0x000000ff       )
    };
}

inline rgba get_rgb_colour(builtin_greyscale_colour col) {
    const uint8_t visible = static_cast<uint8_t>((static_cast<uint16_t>(col) & 0xff00) >> 8);
    return rgba {
        .r = visible,
        .g = visible,
        .b = visible,
        .a = static_cast<uint8_t>(static_cast<uint16_t>(col) & 0x00ff)
    };
}

inline rgba get_rgb_colour(builtin_binary_colour col) {
    const uint8_t visible = static_cast<uint8_t>((col != builtin_binary_colour::black) ? 255 : 0);
    return rgba {
        .r = visible,
        .g = visible,
        .b = visible,
        .a = 255
    };
}

} // namespace rol
