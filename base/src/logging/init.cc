#include "rossignol/utils/logging/init.hh"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>


namespace {

spdlog::level::level_enum get_log_level(rol::log_verbosity verbosity) {
    switch(verbosity) {
        case rol::log_verbosity::low:       return spdlog::level::level_enum::warn;
        case rol::log_verbosity::high:      return spdlog::level::level_enum::debug;
        case rol::log_verbosity::very_high: return spdlog::level::level_enum::trace;
        default: return spdlog::level::level_enum::info;
    }
}

} // anonymous namespace


namespace rol {

void init_logger(log_verbosity verbosity) {
    std::shared_ptr<spdlog::logger> default_logger = spdlog::stderr_color_mt("rossignol");
    default_logger->set_level(::get_log_level(verbosity));
    // Multi-threaded ? => %t
    // Source file ? => %!@%g:%#
    default_logger->set_pattern("[%^%l%$] %v", spdlog::pattern_time_type::local);
    spdlog::set_default_logger(default_logger);
}

} // namespace rol
