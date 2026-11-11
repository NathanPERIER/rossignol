
#pragma once

#include <filesystem>
#include <variant>

#include "rossignol/image/image.hh"


namespace rol {

using parsed_image_t = std::variant<rol::rgb_image, rol::greyscale_image>;

parsed_image_t parse_image(std::span<const uint8_t> raw);
parsed_image_t load_image(const std::filesystem::path& path);

rol::rgb_image parse_image_rgba(std::span<const uint8_t> raw);
rol::rgb_image load_image_rgba(const std::filesystem::path& path);

std::vector<uint8_t> dump_image(rol::rgb_image& img);
std::vector<uint8_t> dump_image(rol::greyscale_image& img);

void write_image(rol::rgb_image& img, const std::filesystem::path& path);
void write_image(rol::greyscale_image& img, const std::filesystem::path& path);

} // namespace rol
