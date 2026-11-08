
#pragma once

#include <cstdint>
#include <filesystem>
#include <span>
#include <vector>


namespace rol {

void write_to_file(const std::filesystem::path& filepath, std::string_view text);
void write_to_file(const std::filesystem::path& filepath, std::span<const uint8_t> binary);

std::string read_file_text(const std::filesystem::path& filepath);
std::vector<uint8_t> read_file_binary(const std::filesystem::path& filepath);

} // namespace rol
