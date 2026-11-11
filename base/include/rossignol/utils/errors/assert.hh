
#pragma once

#include <source_location>
#include <stdexcept>


namespace rol::detail {

/// @brief this exception type is intended for errors in the logic of the program (i.e. bugs)
class assertion_error: public std::logic_error {
public:
    assertion_error(const std::string& message): std::logic_error(message) {}
};

} // namespace rol::detail


#define ROL_ASSERT(condition, msg_format, ...) {                                                                                                                    \
    if(!(condition)) {                                                                                                                                              \
        std::source_location loc;                                                                                                                                   \
        throw rol::detail::assertion_error(fmt::format("{}@{}:{} Assertion failed: " msg_format, loc.function_name(), loc.file_name(), loc.line(), ##__VA_ARGS__)); \
    }                                                                                                                                                               \
} static_assert(true)
