#include "rossignol/colour/generic.hh"

#include "rossignol/colour/conv/greyscale.hh"
#include "rossignol/colour/conv/numeric.hh"


namespace {

template <typename OutColour>
struct decay_colour_impl {
    template <typename InColour>
    requires(!std::same_as<InColour, OutColour>)
    OutColour operator()(const InColour& col) const {
        return rol::colour_conv<OutColour>(col);
    }

    OutColour operator()(const OutColour& col) const {
        return col;
    }
};


/*--------------------+
|  Greyscale          |
+--------------------*/

template<>
template<>
rol::greyscalea decay_colour_impl<rol::greyscalea>::operator()(const rol::rgba& col) const {
    if(col.r != col.g || col.g != col.b) {
        throw std::runtime_error("Cannot implicitely convert RGB colour to greyscale");
    }
    return rol::greyscalea {
        .grey = col.r,
        .a = col.a
    };
}


/*--------------------+
|  Binary             |
+--------------------*/

template<>
template<>
bool decay_colour_impl<bool>::operator()(const rol::rgba& col) const {
    if(col.r != col.g || col.g != col.b || col.r != 0 || col.r != 255) {
        throw std::runtime_error("Cannot implicitely convert RGB colour to binary colour");
    }
    return (col.r == 255);
}

template<>
template<>
bool decay_colour_impl<bool>::operator()(const rol::greyscalea& col) const {
    if(col.grey == 255) {
        return true;
    }
    if(col.grey == 0) {
        return false;
    }
    throw std::runtime_error("Cannot implicitely convert greyscale colour to binary colour");
}

template<>
template<>
bool decay_colour_impl<bool>::operator()(const uint8_t& col) const {
    if(col == 255) {
        return true;
    }
    if(col == 0) {
        return false;
    }
    throw std::runtime_error("Cannot implicitely convert channel value to binary colour");
}

template<>
template<>
bool decay_colour_impl<bool>::operator()(const double& col) const {
    if(col == 1.0) {
        return true;
    }
    if(col == 0.0) {
        return false;
    }
    throw std::runtime_error("Cannot implicitely convert coefficient to binary colour");
}


/*--------------------+
|  Channel value      |
+--------------------*/

template<>
template<>
uint8_t decay_colour_impl<uint8_t>::operator()(const rol::rgba& col) const {
    if(col.r != col.g || col.g != col.b) {
        throw std::runtime_error("Cannot implicitely convert RGB colour to channel value");
    }
    return col.r;
}

template<>
template<>
uint8_t decay_colour_impl<uint8_t>::operator()(const rol::greyscalea& col) const {
    return col.grey;
}


/*--------------------+
|  Coefficient        |
+--------------------*/

template<>
template<>
double decay_colour_impl<double>::operator()(const rol::rgba& col) const {
    if(col.r != col.g || col.g != col.b) {
        throw std::runtime_error("Cannot implicitely convert RGB colour to coefficient");
    }
    return rol::colour_conv<double>(col.r);
}

template<>
template<>
double decay_colour_impl<double>::operator()(const rol::greyscalea& col) const {
    return rol::colour_conv<double>(col.grey);
}

} // anonymous namespace


namespace rol {

template <typename Colour>
requires(variant_alternative<Colour, colour::variant>)
Colour colour::decay_to() const {
    return std::visit(::decay_colour_impl<Colour>{}, _col);
}

template rol::rgba colour::decay_to() const;
template rol::greyscalea colour::decay_to() const;
template bool colour::decay_to() const;
template uint8_t colour::decay_to() const;
template double colour::decay_to() const;

} // namespace rol
