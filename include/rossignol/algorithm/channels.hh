
#pragma once

#include "rossignol/algorithm/map_pixels.hh"
#include "rossignol/math/rect.hh"
#include "rossignol/params/layer_name.hh"
#include "rossignol/utils/typing/functional.hh"


namespace rol::algo::detail {

template <typename Colour, layer_name Layer>
struct layer_extractor;

template <>
struct layer_extractor<rgba, layer_name::red> {
    uint8_t& operator()(rgba& col) const { return col.r; }
    uint8_t operator()(const rgba& col) const { return col.r; }
};

template <>
struct layer_extractor<rgba, layer_name::green> {
    uint8_t& operator()(rgba& col) const { return col.g; }
    uint8_t operator()(const rgba& col) const { return col.g; }
};

template <>
struct layer_extractor<rgba, layer_name::blue> {
    uint8_t& operator()(rgba& col) const { return col.b; }
    uint8_t operator()(const rgba& col) const { return col.b; }
};

template <>
struct layer_extractor<rgba, layer_name::alpha> {
    uint8_t& operator()(rgba& col) const { return col.a; }
    uint8_t operator()(const rgba& col) const { return col.a; }
};

template <>
struct layer_extractor<greyscalea, layer_name::grey> {
    uint8_t& operator()(greyscalea& col) const { return col.grey; }
    uint8_t operator()(const greyscalea& col) const { return col.grey; }
};

template <>
struct layer_extractor<greyscalea, layer_name::alpha> {
    uint8_t& operator()(greyscalea& col) const { return col.a; }
    uint8_t operator()(const greyscalea& col) const { return col.a; }
};

template <layer_name Layer, typename Colour>
concept extractible_layer =
    yield_invocable<const layer_extractor<Colour, Layer>, uint8_t, const Colour&>
    && yield_invocable<const layer_extractor<Colour, Layer>, uint8_t&, Colour&>;


template <layer_name Layer, typename Colour>
bool insert_channel_inplace_impl(basic_image<Colour>& img, const layer& layr, const math::vec2i& offset) {
    static constexpr layer_extractor<Colour, Layer> extractor {};
    // This is the intersection area in the channel's frame of reference
    const math::rect intersection = math::rect(layr.size()).intersect(math::rect(img.size()) - offset);
    if(intersection.empty()) {
        return false;
    }
    // Beginning of the area to copy from the channel
    const math::vec2u channel_start {
        .x = static_cast<std::size_t>(intersection.x1()),
        .y = static_cast<std::size_t>(intersection.y1())
    };
    // Size of the area to copy
    const math::vec2u area_size = intersection.size();
    // Beginning of the area where the channel is inserted in the image
    const math::vec2u image_start {
        .x = (offset.x >= 0) ? static_cast<std::size_t>(offset.x) : 0,
        .y = (offset.y >= 0) ? static_cast<std::size_t>(offset.y) : 0
    };
    // Copy to the requested layer
    for(std::size_t y = 0; y < area_size.y; y++) {
        std::span<Colour> image_row = img[y + image_start.y].subspan(image_start.x);
        std::span<const uint8_t> channel_row = layr[y + channel_start.y].subspan(channel_start.x);
        for(std::size_t x = 0; x < area_size.x; x++) {
            extractor(image_row[x]) = channel_row[x];
        }
    }
    return true;
}

} // namespace rol::algo::detail


namespace rol::algo {

/*--------------------+
|  RGBA               |
+--------------------*/

template<layer_name Layer>
// requires (detail::extractible_layer<Layer, rol::rgba>)
inline layer extract_channel(const rgb_image& img) {
    return map_pixels<uint8_t>(img, detail::layer_extractor<rol::rgba, Layer> {});
}

template<layer_name Layer>
// requires (detail::extractible_layer<Layer, rol::rgba>)
inline bool insert_channel_inplace(rgb_image& img, const layer& layr, const math::vec2i& offset) {
    return detail::insert_channel_inplace_impl<Layer>(img, layr, offset);
}


/*--------------------+
|  GreyscaleA         |
+--------------------*/

template<layer_name Layer>
// requires (detail::extractible_layer<Layer, rol::greyscalea>)
inline layer extract_channel(const greyscale_image& img) {
    return map_pixels<uint8_t>(img, detail::layer_extractor<rol::greyscalea, Layer> {});
}

template<layer_name Layer>
// requires (detail::extractible_layer<Layer, rol::rgba>)
inline bool insert_channel_inplace(greyscale_image& img, const layer& layr, const math::vec2i& offset) {
    return detail::insert_channel_inplace_impl<Layer>(img, layr, offset);
}


/*--------------------+
|  Binary             |
+--------------------*/

layer extract_channel(const binary_image& img);


/*--------------------+
|  Coefficients       |
+--------------------*/

layer to_channel(const coefficient_plane& img);

} // namespace rol::algo
