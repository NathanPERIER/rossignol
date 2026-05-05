
#pragma once

#include "rossignol/image/image.hh"


namespace rol::detail {

coefficient_plane to_coefficients(const layer& img);

} // namespace rol::detail


namespace rol {

coefficient_plane to_coefficients(const image& img);

} // namespace rol
