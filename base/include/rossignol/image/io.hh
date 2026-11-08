
#pragma once

#include "rossignol/image/image.hh"


namespace rol {

image parse_image(std::span<const uint8_t> raw);

std::vector<uint8_t> dump_image(image& img);
std::vector<uint8_t> dump_image(rol::rgb_image& img);
std::vector<uint8_t> dump_image(rol::greyscale_image& img);

} // namespace rol
