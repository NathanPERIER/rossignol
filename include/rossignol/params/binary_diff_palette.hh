
#pragma once

#include "rossignol/colour/rgb.hh"


namespace rol::params {

struct binary_diff_palette {
    rgba zero;
    rgba one;
    rgba added;
    rgba removed;
    rgba created_one;
    rgba created_zero;
    rgba deleted_one;
    rgba deleted_zero;
    rgba empty = rgba{ .r = 0, .g = 0, .b = 0, .a = 0 };

    binary_diff_palette invert() const {
        return binary_diff_palette {
            .zero         = one,
            .one          = zero,
            .added        = removed,
            .removed      = added,
            .created_one  = deleted_one,
            .created_zero = deleted_zero,
            .deleted_one  = created_one,
            .deleted_zero = created_zero,
            .empty        = empty
        };
    }

    const rgba& get_diff_colour(bool original, bool updated) const {
        if(original) {
            if(updated) {
                return one;
            }
            return removed;
        }
        if(updated) {
            return added;
        }
        return zero;
    }

    const rgba& get_created_colour(bool value) {
        return value ? created_one : created_zero;
    }

    const rgba& get_deleted_colour(bool value) {
        return value ? deleted_one : deleted_zero;
    }
};

constexpr binary_diff_palette programmer_diff_palette {
    .zero         = rgba{ .r =   0, .g =   0, .b =   0, .a = 255 },
    .one          = rgba{ .r = 255, .g = 255, .b = 255, .a = 255 },
    .added        = rgba{ .r =   0, .g = 255, .b =   0, .a = 255 },
    .removed      = rgba{ .r = 255, .g =   0, .b =   0, .a = 255 },
    .created_one  = rgba{ .r =   0, .g = 127, .b =   0, .a = 255 },
    .created_zero = rgba{ .r =   0, .g =  63, .b =   0, .a = 255 },
    .deleted_one  = rgba{ .r = 127, .g =   0, .b =   0, .a = 255 },
    .deleted_zero = rgba{ .r =  63, .g =   0, .b =   0, .a = 255 }
};

} // namespace rol::params


namespace rol::defaults {

constexpr params::binary_diff_palette binary_diff_palette = params::programmer_diff_palette;

} // namespace rol::defaults
