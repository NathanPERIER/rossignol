
#pragma once

#include <variant>

#include "rossignol/image/image.hh"


namespace rol {

using parsed_image_t = std::variant<rol::rgb_image, rol::greyscale_image>;

parsed_image_t parse_image(std::span<const uint8_t> raw);

std::vector<uint8_t> dump_image(rol::rgb_image& img);
std::vector<uint8_t> dump_image(rol::greyscale_image& img);

} // namespace rol
