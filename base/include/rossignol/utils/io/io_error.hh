
#pragma once

#include <stdexcept>


namespace rol {

class io_error: public std::runtime_error {
public:
    io_error(const std::string& message): std::runtime_error(message) {}
};

} // namespace rol
