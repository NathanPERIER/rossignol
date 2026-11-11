#include "rossignol/generic/sinks/file.hh"

#include "rossignol/image/io.hh"
#include "rossignol/utils/typing/any_of.hh"
#include "rossignol/utils/errors/check.hh"

namespace {

template <typename Colour>
requires(rol::any_of<Colour, rol::rgba, rol::greyscalea>)
void write_file_impl(const rol::basic_image<Colour>& img, const std::filesystem::path file_path) {
    rol::basic_image<Colour> img_clone = img.clone();
    rol::write_image(img_clone, file_path);
}

} // anonymous namespace


namespace rol::generic {

void file_sink::operator()(const rgb_image& img) const {
    return ::write_file_impl(img, _file_path);
}

void file_sink::operator()(const greyscale_image& img) const {
    return ::write_file_impl(img, _file_path);
}

void file_sink::operator()(const binary_image&) const {
    throw rol::input_error("Cannot write a binary image directly to a file (maybe convert to greyscale first ?)");
}

void file_sink::operator()(const layer&) const {
    throw rol::input_error("Cannot write a channel directly to a file (maybe convert to greyscale first ?)");
}

void file_sink::operator()(const coefficient_plane&) const{
    throw rol::input_error("Cannot write a coefficient plane directly to a file (maybe convert to greyscale first ?)");
}

void file_sink::operator()(const image& img) const {
    return std::visit([this](const auto& img) { return operator()(img); }, img);
}

} // namespace rol::generic