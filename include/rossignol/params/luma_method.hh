
#pragma once


namespace rol {

enum class luma_method {
    sdtv,
    hdtv,
    hdr
};

} // namespace rol


namespace rol::defaults {

constexpr luma_method greyscale_method = luma_method::sdtv;

} // namespace rol::defaults
