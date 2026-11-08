
#pragma once

#include <concepts>


namespace rol {

template <typename T, typename... U>
concept any_of = (std::same_as<T, U> || ...);

} // namespace rol
