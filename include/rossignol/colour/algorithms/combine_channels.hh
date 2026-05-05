
#pragma once

#include "rossignol/colour/rgb.hh"


namespace rol::detail {

template <uint32_t Factor, uint32_t RedCoeff, uint32_t GreenCoeff, uint32_t BlueCoeff>
requires((Factor >= 1) && (0 <= RedCoeff && RedCoeff <= Factor) && (0 <= GreenCoeff && GreenCoeff <= Factor) && (0 <= BlueCoeff && BlueCoeff <= Factor) && (RedCoeff + BlueCoeff + GreenCoeff == Factor))
uint8_t combine_channels(const rgba& col) {
    return static_cast<uint8_t>((
          static_cast<uint32_t>(col.r) * RedCoeff
        + static_cast<uint32_t>(col.g) * GreenCoeff
        + static_cast<uint32_t>(col.b) * BlueCoeff
    ) / Factor);
}

} // namespace rol::detail
