#include "rossignol/params/generic/size.hh"


namespace {

class compute_length {
public:
    compute_length(std::size_t image_length): _image_length(image_length) {}

    std::size_t operator()(const rol::generic::pixel_count& count) const {
        if(count.value < 0) {
            throw std::runtime_error("Length in pixels must be greater than zero");
        }
        return static_cast<std::size_t>(count.value);
    }

    std::size_t operator()(const rol::generic::percentage& pct) const {
        if(pct.value < 0) {
            throw std::runtime_error("Length percentage must be greater than zero");
        }
        return pct.apply(_image_length);
    }

private:
    std::size_t _image_length;

};

} // anonymous namespace


namespace rol::generic {

math::vec2u compute_size(image_length x, image_length y, math::vec2u image_size) {
    return math::vec2u {
        .x = std::visit(::compute_length(image_size.x), x),
        .y = std::visit(::compute_length(image_size.y), y)
    };
}

} // namespace rol::generic
