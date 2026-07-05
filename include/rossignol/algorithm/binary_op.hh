
#pragma once

#include "rossignol/image/image.hh"
#include "rossignol/math/vec2.hh"


namespace rol::algo {

void bitwise_and_inplace(layer& img, uint8_t mask);
void bitwise_or_inplace(layer& img, uint8_t mask);
void bitwise_xor_inplace(layer& img, uint8_t mask);

bool bitwise_and_inplace(layer& img, const layer& mask, const math::vec2i& offset);
bool bitwise_or_inplace(layer& img, const layer& mask, const math::vec2i& offset);
bool bitwise_xor_inplace(layer& img, const layer& mask, const math::vec2i& offset);

bool bitwise_and_inplace(binary_image& img, const binary_image& mask, const math::vec2i& offset);
bool bitwise_or_inplace(binary_image& img, const binary_image& mask, const math::vec2i& offset);
bool bitwise_xor_inplace(binary_image& img, const binary_image& mask, const math::vec2i& offset);

} // namespace rol::algo
