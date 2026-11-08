#include "rossignol/generic/sinks/file.hh"

#include "rossignol/image/io.hh"
#include "rossignol/utils/io/file.hh"
#include "rossignol/utils/typing/any_of.hh"

namespace {

template <typename Colour>
requires(rol::any_of<Colour, rol::rgba, rol::greyscalea>)
void write_file_impl(const rol::basic_image<Colour>& img, const std::filesystem::path file_path) {
    rol::basic_image<Colour> img_clone = img.clone();
    const std::vector<uint8_t> dump = rol::dump_image(img_clone);
    rol::write_to_file(file_path, dump);
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
    throw std::runtime_error("Cannot write a binary image directly to a file (maybe convert to greyscale first ?)");
}

void file_sink::operator()(const layer&) const {
    throw std::runtime_error("Cannot write a channel directly to a file (maybe convert to greyscale first ?)");
}

void file_sink::operator()(const coefficient_plane&) const{
    throw std::runtime_error("Cannot write a coefficient plane directly to a file (maybe convert to greyscale first ?)");
}

void file_sink::operator()(const image& img) const {
    return std::visit([this](const auto& img) { return operator()(img); }, img);
}

} // namespace rol::generic