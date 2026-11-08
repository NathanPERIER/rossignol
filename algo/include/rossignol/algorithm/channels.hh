
#pragma once

#include "rossignol/algorithm/map_pixels.hh"
#include "rossignol/algorithm/overlaying.hh"
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
    return overlayed_images_intersection(img, layr, offset, [](const math::vec2u& size, basic_image<Colour>& img, const math::vec2u& img_start, const layer& layr, const math::vec2u& layr_start) {
        for(std::size_t y = 0; y < size.y; y++) {
            std::span<Colour> image_row = img[y + img_start.y].subspan(img_start.x);
            std::span<const uint8_t> channel_row = layr[y + layr_start.y].subspan(layr_start.x);
            for(std::size_t x = 0; x < size.x; x++) {
                extractor(image_row[x]) = channel_row[x];
            }
        }
    });
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
