
#pragma once

#include <variant>


namespace rol::detail {

template <typename T, typename Variant>
struct is_alternative: public std::false_type {};

template <typename T, typename... V>
struct is_alternative<T, std::variant<V...>> {
    static constexpr bool value = (... || std::is_same_v<T, V>);
};

} // namespace rol::detail


namespace rol {

template <typename T, typename V>
concept variant_alternative = detail::is_alternative<T, V>::value;

} // namespace rol
