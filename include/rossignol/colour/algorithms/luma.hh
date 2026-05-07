
#pragma once

#include "rossignol/colour/algorithms/combine_channels.hh"
#include "rossignol/params/luma_method.hh"


namespace rol {

template <luma_method Luma>
uint8_t luma(const rgba& col) {
    if constexpr (Luma == luma_method::sdtv) {
        return detail::combine_channels<10000, 2990, 5870, 1140>(col);
    } else if constexpr (Luma == luma_method::hdtv) {
        return detail::combine_channels<10000, 2126, 7152,  722>(col);
    } else {
        static_assert(Luma == luma_method::hdr);
        return detail::combine_channels<10000, 2627, 6780,  593>(col);
    }
}

} // namespace rol
