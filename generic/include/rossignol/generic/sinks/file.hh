
#pragma once

#include <filesystem>

#include "rossignol/image/image.hh"


namespace rol::generic {

class file_sink {
public:
    file_sink(std::filesystem::path file_path): _file_path(std::move(file_path)) {}

    void operator()(const rgb_image& img) const;
    void operator()(const greyscale_image& img) const;

    void operator()(const image& img) const;

private:
    std::filesystem::path _file_path;

    void operator()(const binary_image&) const;
    void operator()(const layer&) const;
    void operator()(const coefficient_plane&) const;
};

} // namespace rol::generic