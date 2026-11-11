
#pragma once


namespace rol {

enum class log_verbosity {
    low,
    normal,
    high,
    very_high
};

void init_logger(log_verbosity verbosity = log_verbosity::normal);

} // namespace rol
