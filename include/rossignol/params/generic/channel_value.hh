
#pragma once

#include <variant>

#include "rossignol/colour/conv/numeric.hh"
#include "rossignol/params/limits.hh"


namespace rol::generic {

class channel_value {
public:
    constexpr channel_value(uint8_t value): _value(value) {}
    constexpr channel_value(double value): _value(value) {}

    template <any_of<uint8_t, double> ChannelType>
    ChannelType as() const {
        constexpr struct {
            ChannelType operator()(const uint8_t& value) const {
                if constexpr (std::same_as<ChannelType, uint8_t>) {
                    return value;
                } else {
                    static_assert(std::same_as<ChannelType, double>);
                    return colour_conv<ChannelType>(value);
                }
            }
            ChannelType operator()(const double& value) const {
                if constexpr (std::same_as<ChannelType, double>) {
                    return value;
                } else {
                    static_assert(std::same_as<ChannelType, uint8_t>);
                    return colour_conv<ChannelType>(value);
                }
            }
        } visitor;
        return std::visit(visitor, _value);
    }

private:
    std::variant<uint8_t, double> _value;
};

} // namespace rol::generic


namespace rol::generic::defaults {

constexpr channel_value channel_min(rol::defaults::channel_limits<double>::min);
constexpr channel_value channel_max(rol::defaults::channel_limits<double>::max);

} // namespace rol::generic::defaults
