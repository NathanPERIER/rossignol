#include "rossignol/utils/io/file.hh"

#include <fstream>
#include <iterator>

#include <fmt/std.h>


namespace {

constexpr std::size_t read_size = 1024;

} // anonymous namespace


namespace rol {

void write_to_file(const std::filesystem::path& filepath, std::string_view text) {
    std::ofstream out(filepath, std::ios::out);
    out.write(text.data(), text.size());
}

void write_to_file(const std::filesystem::path& filepath, std::span<const uint8_t> binary) {
    std::ofstream out(filepath, std::ios::out | std::ios::binary);
    out.write(reinterpret_cast<const char*>(binary.data()), binary.size());
}


std::string read_file_text(const std::filesystem::path& filepath) {
    std::string res;
    std::ifstream in(filepath, std::ios::in);
    if(in.fail()) {
        throw std::runtime_error(fmt::format("Could not open file {} for reading", filepath));
    }
    while(!in.eof()) {
        const std::size_t initial_size = res.size();
        // TODO use resize_and_overwrite in C++23
        res.resize(initial_size + ::read_size);
        in.read(reinterpret_cast<char*>(res.data()) + initial_size, ::read_size);
        const std::size_t read_count = static_cast<std::size_t>(in.gcount());
        if(read_count < ::read_size) {
            res.resize(initial_size + read_count);
            break;
        }
    }
    if(in.bad() || !in.eof()) {
        throw std::runtime_error(fmt::format("Error while reading file {}", filepath));
    }
    return res;
}

std::vector<uint8_t> read_file_binary(const std::filesystem::path& filepath) {
    std::vector<uint8_t> res;
    std::ifstream in(filepath, std::ios::in | std::ios::binary);
    if(in.fail()) {
        throw std::runtime_error(fmt::format("Could not open file {} for reading", filepath));
    }
    while(!in.eof()) {
        const std::size_t initial_size = res.size();
        res.resize(initial_size + ::read_size);
        in.read(reinterpret_cast<char*>(res.data()) + initial_size, ::read_size);
        const std::size_t read_count = static_cast<std::size_t>(in.gcount());
        if(read_count < ::read_size) {
            res.resize(initial_size + read_count);
            break;
        }
    }
    if(in.bad() || !in.eof()) {
        throw std::runtime_error(fmt::format("Error while reading file {}", filepath));
    }
    return res;
}

} // namespace rol
