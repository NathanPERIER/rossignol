
#pragma once


namespace rol {

struct hsv {
    double h;
    double s;
    double v;

    bool operator==(const hsv& col) const = default;
};

} // namespace rol

