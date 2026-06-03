#include "rossignol/algorithm/diff.hh"

#include "rossignol/math/rect.hh"


namespace rol::algo {

// layer distance(const layer& l1, const layer& l2, math::vec2i offset);

// coefficient_plane distance(const coefficient_plane& p1, const coefficient_plane& p2, math::vec2i offset);

// coefficient_plane diff(const coefficient_plane& original, const coefficient_plane& updated, math::vec2i offset);

rgb_image diff(const binary_image& original, const binary_image& updated, math::vec2i offset, const params::binary_diff_palette& palette) {
    const math::rect original_rect = math::rect(original.size());
    const math::rect updated_rect = math::rect(updated.size()) + offset;
    const math::rect images_union = original_rect.encompass(updated_rect);
    const math::rect images_intersection = original_rect.intersect(updated_rect);
    if(images_intersection.empty()) {
        throw std::runtime_error("Images do not intersect with the current offset setting");
    }
    const math::vec2u intersection_size = images_intersection.size();
    // Point at which the intersection starts in the result image
    const math::vec2u res_start {
        .x = static_cast<std::size_t>(std::abs(offset.x)),
        .y = static_cast<std::size_t>(std::abs(offset.y))
    };
    // Point at which the intersection starts in the original image
    const math::vec2u original_start {
        .x = (offset.x > 0) ? static_cast<std::size_t>(offset.x) : 0,
        .y = (offset.y > 0) ? static_cast<std::size_t>(offset.y) : 0
    };
    // Point at which the intersection starts in the updated image
    const math::vec2u updated_start {
        .x = (offset.x < 0) ? static_cast<std::size_t>(-offset.x) : 0,
        .y = (offset.y < 0) ? static_cast<std::size_t>(-offset.y) : 0
    };

    rgb_image res(images_union.size());
    for(std::size_t y = 0; y < intersection_size.y; y++) {
        std::span<rgba> res_row = res[res_start.y + y];
        std::span<const bool> original_row = original[original_start.y + y];
        std::span<const bool> updated_row = updated[updated_start.y + y];
        for(std::size_t x = 0; x < intersection_size.x; x++) {
            res_row[res_start.x + x] = palette.get_diff_colour(original_row[original_start.x + x], updated_row[updated_start.x + x]);
        }
    }
    return res;
}

} // namespace rol::algo

