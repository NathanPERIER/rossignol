
#pragma once


namespace rol {

enum class random_method {
    uniform,
    gaussian
};

} // namespace rol


namespace rol::defaults {

constexpr rol::random_method random_method = rol::random_method::uniform;

constexpr double bernoulli_probability = 0.5;

} // namespace rol::defaults
