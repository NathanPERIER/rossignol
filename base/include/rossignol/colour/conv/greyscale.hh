
#pragma once

#include "rossignol/colour/greyscale.hh"
#include "rossignol/colour/rgb.hh"
#include "rossignol/params/luma_method.hh"
#include "rossignol/utils/typing/any_of.hh"


namespace rol::detail {

template <uint32_t Factor, uint32_t RedCoeff, uint32_t GreenCoeff, uint32_t BlueCoeff>
requires((Factor >= 1) && (0 <= RedCoeff && RedCoeff <= Factor) && (0 <= GreenCoeff && GreenCoeff <= Factor) && (0 <= BlueCoeff && BlueCoeff <= Factor) && (RedCoeff + BlueCoeff + GreenCoeff == Factor))
inline uint8_t combine_channels(const rgba& col) {
    return static_cast<uint8_t>((
          static_cast<uint32_t>(col.r) * RedCoeff
        + static_cast<uint32_t>(col.g) * GreenCoeff
        + static_cast<uint32_t>(col.b) * BlueCoeff
    ) / Factor);
}

template <double RedCoeff, double GreenCoeff, double BlueCoeff>
requires((0.0 <= RedCoeff && RedCoeff <= 1.0) && (0.0 <= GreenCoeff && GreenCoeff <= 1.0) && (0.0 <= BlueCoeff && BlueCoeff <= 1.0) && (RedCoeff + BlueCoeff + GreenCoeff == 1.0))
inline uint8_t combine_channels(const rgba& col) {
    return static_cast<double>(col.r) * RedCoeff
         + static_cast<double>(col.g) * GreenCoeff
         + static_cast<double>(col.b) * BlueCoeff;
}

} // namespace rol::detail


namespace rol {

template <luma_method Luma>
inline uint8_t luma(const rgba& col) {
    if constexpr (Luma == luma_method::sdtv) {
        return detail::combine_channels<10000, 2990, 5870, 1140>(col);
    } else if constexpr (Luma == luma_method::hdtv) {
        return detail::combine_channels<10000, 2126, 7152,  722>(col);
    } else {
        static_assert(Luma == luma_method::hdr);
        return detail::combine_channels<10000, 2627, 6780,  593>(col);
    }
}

template <luma_method Luma>
inline double luma_coeff(const rgba& col) {
    if constexpr (Luma == luma_method::sdtv) {
        return detail::combine_channels<0.2990, 0.5870, 0.1140>(col);
    } else if constexpr (Luma == luma_method::hdtv) {
        return detail::combine_channels<0.2126, 0.7152, 0.0722>(col);
    } else {
        static_assert(Luma == luma_method::hdr);
        return detail::combine_channels<0.2627, 0.6780, 0.0593>(col);
    }
}


template <typename DestColour>
requires(any_of<DestColour, rgba>)
DestColour colour_conv(const greyscalea& col);

template <>
inline rgba colour_conv<rgba>(const greyscalea& col) {
    return rgba {
        .r = col.grey,
        .g = col.grey,
        .b = col.grey,
        .a = col.a
    };
}

} // namespace rol
