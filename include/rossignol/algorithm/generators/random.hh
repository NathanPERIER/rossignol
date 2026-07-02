
#pragma once

#include "rossignol/image/image.hh"
#include "rossignol/params/random.hh"


namespace rol::algo {

binary_image random_binary_image(const math::vec2u& size, double bernoulli_probability = rol::defaults::bernoulli_probability);

template<random_method Method = rol::defaults::random_method>
layer random_layer(const math::vec2u& size);

template<random_method Method = rol::defaults::random_method>
coefficient_plane random_coefficient_plane(const math::vec2u& size);

// TODO In the future, the alpha channel shall also be random
template<random_method Method = rol::defaults::random_method>
greyscale_image random_greyscale_image(const math::vec2u& size);

// TODO In the future, the alpha channel shall also be random
template<random_method Method = rol::defaults::random_method>
rgb_image random_rgb_image(const math::vec2u& size);

} // namespace rol::algo
