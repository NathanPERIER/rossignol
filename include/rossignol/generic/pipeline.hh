
#pragma once

#include "rossignol/image/image.hh"
#include "rossignol/utils/typing/functional.hh"
#include "rossignol/utils/typing/variant.hh"


namespace rol::generic::detail {

/// @brief matches one of the image types in the generic image variant
template <typename Image>
concept pipeline_specific_image = rol::variant_alternative<Image, rol::image>;

/// @brief return type of an operator, either the geenric image or one of its sub-types
template <typename Image>
concept pipeline_return_type = std::same_as<rol::image, Image> || pipeline_specific_image<Image>;

/// @brief helper that matches any callable returning a generic image or one of its sub-types
template <typename Generator, typename... Args>
concept pipeline_image_generator = std::invocable<const Generator, Args...> && pipeline_return_type<std::invoke_result_t<const Generator, Args...>>;

/// @brief operator that takes a generic image as input
template <typename Op>
concept pipeline_generic_image_operator = pipeline_image_generator<Op, const rol::image&>;

/// @brief operator that takes a sub-type of the generic image as input
template <typename Op, typename Colour>
concept pipeline_specific_image_operator = pipeline_specific_image<rol::basic_image<Colour>> && pipeline_image_generator<Op, const rol::basic_image<Colour>&>;

/// @brief sink that takes a generic image as input
template <typename Sink>
concept pipeline_generic_image_sink = rol::yield_invocable<Sink, void, const rol::image&>;


/// @brief sink that takes a generic image as input
template <typename Sink, typename Colour>
concept pipeline_specific_image_sink = pipeline_specific_image<rol::basic_image<Colour>> && rol::yield_invocable<Sink, void, const rol::basic_image<Colour>&>;

} // namespace rol::generic::detail


namespace rol::generic {

/// @brief enables "piping" a generic image to an operator
/// @tparam Op the operator type
/// @return the result of the operator (either a generic image or a sub-type)
template <detail::pipeline_generic_image_operator Op>
auto operator|(const rol::image& img, const Op& op) {
    return op(img);
}

/// @brief enables "piping" a generic image sub-type to an operator
/// @tparam Colour the type of the pixels in the image
/// @tparam Op the operator type
/// @return the result of the operator (either a generic image or a sub-type)
template <typename Colour, detail::pipeline_specific_image_operator<Colour> Op>
auto operator|(const rol::basic_image<Colour>& img, const Op& op) {
    return op(img);
}

/// @brief enables "piping" a generic image to a sink
/// @tparam Sink the sink type
template <detail::pipeline_generic_image_sink Sink>
void operator>(const rol::image& img, const Sink& sink) {
    sink(img);
}

/// @brief enables "piping" a generic image sub-type to a sink
/// @tparam Colour the type of the pixels in the image
/// @tparam Sink the sink type
template <typename Colour, detail::pipeline_specific_image_sink<Colour> Sink>
auto operator>(const rol::basic_image<Colour>& img, const Sink& sink) {
    sink(img);
}

} // namespace rol::generic
