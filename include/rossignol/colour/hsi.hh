
#pragma once


namespace rol {

struct hsi {
    double h;
    double s;
    double i;

    bool operator==(const hsi& col) const = default;
};

} // namespace rol

