
#pragma once

#include "rossignol/colour/rgb.hh"


namespace rol::detail {

constexpr uint32_t luma_coeff_factor = 10000;

template <uint32_t RedCoeff, uint32_t GreenCoeff, uint32_t BlueCoeff>
requires((0 <= RedCoeff && RedCoeff <= luma_coeff_factor) && (0 <= GreenCoeff && GreenCoeff <= luma_coeff_factor) && (0 <= BlueCoeff && BlueCoeff <= luma_coeff_factor) && (RedCoeff + BlueCoeff + GreenCoeff == luma_coeff_factor))
uint8_t luma_impl(const rgba& col) {
    return static_cast<uint8_t>((
          static_cast<uint32_t>(col.r) * RedCoeff
        + static_cast<uint32_t>(col.g) * GreenCoeff
        + static_cast<uint32_t>(col.b) * BlueCoeff
    ) / luma_coeff_factor);
}

} // namespace rol::detail


namespace rol {

enum class luma_method {
    sdtv,
    hdtv,
    hdr
};

template <luma_method Luma>
uint8_t luma(const rgba& col) {
    if constexpr (Luma == luma_method::sdtv) {
        return detail::luma_impl<2990, 5870, 1140>(col);
    } else if constexpr (Luma == luma_method::hdtv) {
        return detail::luma_impl<2126, 7152,  722>(col);
    } else {
        static_assert(Luma == luma_method::hdr);
        return detail::luma_impl<2627, 6780,  593>(col);
    }
}

} // namespace rol
