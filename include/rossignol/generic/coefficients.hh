
#pragma once

#include "rossignol/image/image.hh"
#include "rossignol/params/coefficient_type.hh"
#include "rossignol/params/generic/offset.hh"


namespace rol::generic {

class to_coefficients {
public:
    to_coefficients() = default;

    coefficient_plane operator()(const layer& img) const;
    coefficient_plane operator()(const coefficient_plane& img) const { return img.share(); }

    coefficient_plane operator()(const image& img) const;

private:
    coefficient_plane operator()(const rgb_image&) const;
    coefficient_plane operator()(const greyscale_image&) const;
    coefficient_plane operator()(const binary_image&) const;
};


class extract_coefficient {
public:
    extract_coefficient(coefficient_type type): _type(type) {}

    coefficient_plane operator()(const rgb_image& img) const;
    coefficient_plane operator()(const greyscale_image& img) const;

    coefficient_plane operator()(const image& img) const;

private:
    coefficient_type _type;

    coefficient_plane operator()(const layer&) const;
    coefficient_plane operator()(const coefficient_plane&) const;
    coefficient_plane operator()(const binary_image&) const;
};


class insert_coefficient {
public:
    insert_coefficient(coefficient_plane coeffs, coefficient_type type, horizontal_offset x_offset, vertical_offset y_offset):
        _coeffs(std::move(coeffs)), _type(type), _x_offset(x_offset), _y_offset(y_offset) {}
    insert_coefficient(coefficient_plane coeffs, coefficient_type type):
        insert_coefficient(std::move(coeffs), type, defaults::horizontal_offset, defaults::vertical_offset) {}

    rgb_image operator()(const rgb_image& img) const;
    greyscale_image operator()(const greyscale_image& img) const;

    image operator()(const image& img) const;

private:
    coefficient_plane _coeffs;
    coefficient_type _type;
    horizontal_offset _x_offset;
    vertical_offset _y_offset;

    math::vec2i get_offset(const rol::math::vec2u& input_size) const;

    binary_image operator()(const binary_image&) const;
    layer operator()(const layer&) const;
    coefficient_plane operator()(const coefficient_plane&) const;
};

} // namespace rol::generic
