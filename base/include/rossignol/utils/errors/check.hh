
#pragma once

#include <source_location>
#include <stdexcept>

#include <fmt/format.h>


namespace rol {

/// @brief this exception type is intended for user input errors (image content, method parameters, ...)
class input_error: public std::runtime_error {
public:
    input_error(const std::string& message): std::runtime_error(message) {}
};

} // namespace rol


#define ROL_CHECK(condition, msg_format, ...) {                                                                                       \
    if(!(condition)) {                                                                                                                \
        std::source_location loc;                                                                                                     \
        throw rol::input_error(fmt::format("{}@{}:{} " msg_format, loc.function_name(), loc.file_name(), loc.line(), ##__VA_ARGS__)); \
    }                                                                                                                                 \
} static_assert(true)
