
#pragma once

#include "rossignol/image/channel.hh"
#include "rossignol/image/coefficient_plane.hh"


namespace rol::algo {

coefficient_plane to_coefficients(const layer& img);

} // namespace rol::algo
