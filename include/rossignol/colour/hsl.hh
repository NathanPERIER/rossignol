
#pragma once


namespace rol {

struct hsl {
    double h;
    double s;
    double l;

    bool operator==(const hsl& col) const = default;
};

} // namespace rol
