
#pragma once

#include <cmath>


namespace rol::math {

class logistic_function {
public:
    logistic_function(double midpoint = 0.0, double growth_rate = 1.0): _midpoint(midpoint), _growth_rate(growth_rate) {}

    double operator()(double x) const {
        return 1.0 / (1.0 + std::exp(-_growth_rate * (x - _midpoint)));
    }

private:
    double _midpoint;
    double _growth_rate;
};

} // namespace rol::math
