#include "rossignol/params/generic/offset.hh"

#include "rossignol/math/align.hh"


namespace {

class compute_offset_component {
public:
    compute_offset_component(std::size_t object_size, std::size_t reference_size):
        _object_size(object_size), _reference_size(reference_size) {}
    
    int64_t operator()(const rol::generic::pixel_count& count) const {
        return count.value;
    }

    int64_t operator()(const rol::generic::percentage& pct) const {
        return pct.apply(_reference_size);
    }

    int64_t operator()(const rol::horizontal_alignment& align) {
        switch(align) {
            case rol::horizontal_alignment::left:   return rol::math::aligned_horizontal_offset<rol::horizontal_alignment::left>(_object_size, _reference_size);
            case rol::horizontal_alignment::centre: return rol::math::aligned_horizontal_offset<rol::horizontal_alignment::centre>(_object_size, _reference_size);
            case rol::horizontal_alignment::right:  return rol::math::aligned_horizontal_offset<rol::horizontal_alignment::right>(_object_size, _reference_size);
        }
        throw std::runtime_error("Unknown horizontal alignment");
    }

    int64_t operator()(const rol::vertical_alignment& align) {
        switch(align) {
            case rol::vertical_alignment::top:    return rol::math::aligned_vertical_offset<rol::vertical_alignment::top>(_object_size, _reference_size);
            case rol::vertical_alignment::centre: return rol::math::aligned_vertical_offset<rol::vertical_alignment::centre>(_object_size, _reference_size);
            case rol::vertical_alignment::bottom: return rol::math::aligned_vertical_offset<rol::vertical_alignment::bottom>(_object_size, _reference_size);
        }
        throw std::runtime_error("Unknown vertical alignment");
    }

private:
    std::size_t _object_size;
    std::size_t _reference_size;
};

} // anonymous namespace


namespace rol::generic {

math::vec2i compute_offset(horizontal_offset x, vertical_offset y, math::vec2u object_size, math::vec2u reference_size) {
    return math::vec2i {
        .x = std::visit(::compute_offset_component(object_size.x, reference_size.x), x),
        .y = std::visit(::compute_offset_component(object_size.y, reference_size.y), y)
    };
}

} // namespace rol::generic
