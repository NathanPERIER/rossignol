#include "rossignol/algorithm/svg.hh"


namespace rol::algo {

std::string to_svg(const rgb_image& img, const params::svg_options& opts) {
    using namespace std::string_view_literals;
    std::string res = R"__(<?xml version="1.0" encoding="UTF-8" standalone="no"?>)__";
    if(opts.pretty) {
        res.push_back('\n');
    }
    std::back_insert_iterator format_it = std::back_inserter(res);
    res.append(R"__(<svg xmlns="http://www.w3.org/2000/svg" )__"sv);
    if(opts.bounding_box) {
        fmt::format_to(format_it, R"__(viewBox="0 0 {} {}")__", img.width(), img.height());
    } else {
        fmt::format_to(format_it, R"__(width="{}" height="{}")__", img.width(), img.height());
    }
    res.append(R"__( shape-rendering="crispEdges">)__"sv);
    const std::string_view indent = opts.pretty ? "\n    "sv : std::string_view();
    for(std::size_t y = 0; y < img.height(); y++) {
        const std::span<const rgba> line = img[y];
        for(std::size_t x = 0; x < img.width(); x++) {
            if(line[x].a == 0) {
                continue;
            }
            fmt::format_to(format_it, R"__({}<rect x="{}" y="{}" width="1" height="1" fill="{}"/>)__", indent, x, y, line[x]);
        }
    }
    if(opts.pretty) {
        res.push_back('\n');
    }
    res.append("</svg>"sv);
    return res;
}

} // namespace rol::algo
