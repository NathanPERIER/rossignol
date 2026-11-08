
#pragma once


namespace rol {

enum class luma_method {
    sdtv,
    hdtv,
    hdr
};

} // namespace rol


namespace rol::defaults {

constexpr rol::luma_method luma_method = rol::luma_method::sdtv;

} // namespace rol::defaults
