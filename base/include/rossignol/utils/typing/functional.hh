
#pragma once

#include <concepts>


namespace rol {

/// @brief invocable function with exact return type check
template <class F, class Res, class... Args>
concept yield_invocable = std::invocable<F, Args...> && std::same_as<std::invoke_result_t<F, Args...>, Res>;

} // namespace rol
