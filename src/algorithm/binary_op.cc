#include "rossignol/algorithm/binary_op.hh"

#include "rossignol/algorithm/map_pixels.hh"
#include "rossignol/algorithm/overlaying.hh"
#include "rossignol/utils/typing/any_of.hh"


namespace {

template <typename Colour, typename Operator>
requires(rol::any_of<Colour, bool, uint8_t> && rol::yield_invocable<Operator, Colour, Colour, Colour>)
bool bitwise_op_inplace_impl(rol::basic_image<Colour>& img, const rol::basic_image<Colour>& mask, const rol::math::vec2i& offset, const Operator& op) {
    return rol::algo::overlayed_images_intersection(img, mask, offset, 
        [&op](const rol::math::vec2u& size, rol::basic_image<Colour>& img, const rol::math::vec2u& img_start, const rol::basic_image<Colour>& mask, const rol::math::vec2u& mask_start) {
            for(std::size_t y = 0; y < size.y; y++) {
                std::span<Colour> img_row = img[y + img_start.y].subspan(img_start.x);
                std::span<const Colour> mask_row = mask[y + mask_start.y].subspan(mask_start.x);
                for(std::size_t x = 0; x < size.x; x++) {
                    img_row[x] = op(img_row[x], mask_row[x]);
                }
            }
        }
    );
}

} // anonymous namespace


namespace rol::algo {

void bitwise_and_inplace(layer& img, uint8_t mask) {
    edit_pixels_inplace(img, [mask](uint8_t& pixel) {
        pixel = pixel & mask;
    });
}

void bitwise_or_inplace(layer& img, uint8_t mask) {
    edit_pixels_inplace(img, [mask](uint8_t& pixel) {
        pixel = pixel | mask;
    });
}

void bitwise_xor_inplace(layer& img, uint8_t mask) {
    edit_pixels_inplace(img, [mask](uint8_t& pixel) {
        pixel = pixel ^ mask;
    });
}


bool bitwise_and_inplace(layer& img, const layer& mask, const math::vec2i& offset) {
    return ::bitwise_op_inplace_impl(img, mask, offset, [](uint8_t val, uint8_t mask) -> uint8_t { return val & mask; });
}

bool bitwise_or_inplace(layer& img, const layer& mask, const math::vec2i& offset) {
    return ::bitwise_op_inplace_impl(img, mask, offset, [](uint8_t val, uint8_t mask) -> uint8_t { return val | mask; });
}

bool bitwise_xor_inplace(layer& img, const layer& mask, const math::vec2i& offset) {
    return ::bitwise_op_inplace_impl(img, mask, offset, [](uint8_t val, uint8_t mask) -> uint8_t { return val ^ mask; });
}


bool bitwise_and_inplace(binary_image& img, const binary_image& mask, const math::vec2i& offset) {
    return ::bitwise_op_inplace_impl(img, mask, offset, [](bool val, bool mask) -> bool { return val && mask; });
}

bool bitwise_or_inplace(binary_image& img, const binary_image& mask, const math::vec2i& offset) {
    return ::bitwise_op_inplace_impl(img, mask, offset, [](bool val, bool mask) -> bool { return val || mask; });
}

bool bitwise_xor_inplace(binary_image& img, const binary_image& mask, const math::vec2i& offset) {
    return ::bitwise_op_inplace_impl(img, mask, offset, [](bool val, bool mask) -> bool { return val != mask; });
}

} // namespace rol::algo
