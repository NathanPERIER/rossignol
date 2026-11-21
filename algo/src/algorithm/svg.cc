#include "rossignol/algorithm/svg.hh"

#include <unordered_set>

#include "rossignol/utils/errors/assert.hh"


namespace {

/// @brief A point on an SVG path
struct path_point {
    std::size_t x;
    std::size_t y;
    /// @brief The next point on the path, or null if this is the last point
    std::shared_ptr<path_point> next = nullptr;

    static std::shared_ptr<path_point> make(std::size_t x, std::size_t y) {
        std::shared_ptr<path_point> res = std::make_shared<path_point>();
        res->x = x;
        res->y = y;
        return res;
    }
};

/// @brief An SVG path representing a polygon
/// @note In the current context, we assume that there are only right angles within this polygon, thus
///       a valid path must always have at least four nodes (three in general for SVG)
/// @note For consistency, the path is drawn counter-clockwise
struct polygonal_path {
    /// @brief Create a basic rectangular path
    static polygonal_path make_rectangle(std::size_t x1, std::size_t y1, std::size_t x2, std::size_t y2) {
        polygonal_path res;
        res.start = path_point::make(x1, y1);
        path_point* pt = res.start.get();
        pt->next = path_point::make(x1, y2);
        pt = pt->next.get();
        pt->next = path_point::make(x2, y2);
        pt = pt->next.get();
        pt->next = path_point::make(x2, y1);
        res.end = pt->next;
        res.is_simple = true;
        return res;
    }

    std::shared_ptr<path_point> start;
    std::shared_ptr<path_point> end;

    /// @brief this is true as long as we can guarantee that the path is a rectangle
    bool is_simple;
};

/// @brief An area comprised of adjacent pixels of the same colour
/// @note Pixels are only considered adjacent if their edges touch (not their vertices)
struct connected_component {
    /// @brief Create a basic connected component of height 1 with a given length and starting position
    /// @note All complex connected component are built by stacking these simple lines
    static std::shared_ptr<connected_component> make(const rol::rgba& colour, std::size_t x, std::size_t width, std::size_t y) {
        std::shared_ptr<connected_component> res = std::make_shared<connected_component>();
        res->colour = colour;
        res->path = polygonal_path::make_rectangle(x, y, x+width, y+1);
        return res;
    }

    rol::rgba colour;
    polygonal_path path;
    std::vector<polygonal_path> holes;
};

/// @brief Finds the index of the next pixel whose colour is not the same as a given pixel on a row
std::size_t get_next_colour_index(std::span<const rol::rgba> row, std::size_t x) {
    std::size_t next_x = x+1;
    while(next_x < row.size() && row[x] == row[next_x]) {
        next_x++;
    }
    return next_x;
}


/// @brief This structure holds information on a horizontal line of a given connected component that lies on the last processed row
struct bottom_line {
    std::size_t component_idx;
    std::shared_ptr<path_point> start_point;
    std::size_t x_start;
    std::size_t x_end;
};


class svg_generator {
public:
    using bottom_iterator = std::vector<bottom_line>::iterator;

    void process(const rol::rgb_image& img);

    /// @warning Connected components are stored as pointers, some may be null
    const std::vector<std::shared_ptr<connected_component>>& connected_components() const { return _components; }

private:
    std::vector<std::shared_ptr<connected_component>> _components;

    std::vector<bottom_line> _bottom_points;
    std::vector<bottom_line> _next_points;

    std::vector<bottom_iterator> _matching_bottom_lines;

    void add_simple_component(const rol::rgba& col, std::size_t y, std::size_t x, std::size_t next_x);
    void process_first_line(std::span<const rol::rgba> row);

    /// @brief Removes bottom line matches related to components that appear several times
    void remove_multiple_line_matches();

    /// @brief Aggregates a line on the current row with the matched bottom lines
    void aggregate_with_matched_points(std::size_t y, std::size_t x, std::size_t next_x);
};


void svg_generator::add_simple_component(const rol::rgba& col, std::size_t y, std::size_t x, std::size_t next_x) {
    // Create the connected component
    _components.push_back(connected_component::make(col, x, next_x - x, y));
    // Fill bottom line information
    _next_points.push_back(bottom_line {
        .component_idx = _components.size() - 1,
        .start_point = _components.back()->path.start->next,
        .x_start = x,
        .x_end = next_x
    });
}

void svg_generator::process_first_line(std::span<const rol::rgba> row) {
    for(std::size_t x = 0; x < row.size();) {
        if(row[x].a == 0) {
            x++;
            continue;
        }
        const std::size_t next_x = get_next_colour_index(row, x);
        add_simple_component(row[x], 0, x, next_x);
        x = next_x;
    }
}


void svg_generator::remove_multiple_line_matches() {
    if(_matching_bottom_lines.empty()) {
        return;
    }

    std::unordered_set<std::size_t> component_matches;
    std::unordered_set<std::size_t> duplicates;

    for(const bottom_iterator& match: _matching_bottom_lines) {
        const bool inserted = component_matches.insert(match->component_idx).second;
        if(!inserted) {
            duplicates.insert(match->component_idx);
        }
    }

    if(duplicates.empty()) {
        return;
    }

    std::erase_if(_matching_bottom_lines, [&duplicates](const bottom_iterator& match) -> bool {
        return duplicates.contains(match->component_idx);
    });
}


void svg_generator::aggregate_with_matched_points(std::size_t y, std::size_t x, std::size_t next_x) {
    ROL_ASSERT(!_matching_bottom_lines.empty(), "This method is only for cases where we have matching points on the bottom line");

    // This is the index of the only component that will remain
    // The others will be merged into this one and erased
    const std::size_t merged_component_idx = _matching_bottom_lines.front()->component_idx;
    std::unordered_set<std::size_t> erased_component_idx;

    // Close the path on the right
    std::shared_ptr<path_point> last_next = nullptr;
    bool update_last_match = false;
    const bottom_iterator last_match_it = _matching_bottom_lines.back();
    if(next_x == last_match_it->x_end) {
        last_next = last_match_it->start_point->next;
        last_next->y++;
    } else {
        //             |               |
        // start_point O---x--->---x---O--O p2
        //                                |
        //        ------------------------O last_next
        _components[last_match_it->component_idx]->path.is_simple = false;
        std::shared_ptr<path_point> pt = last_match_it->start_point->next;
        std::shared_ptr<path_point> p2 = path_point::make(next_x, pt->y);
        p2->next = pt;
        last_next = path_point::make(next_x, pt->y + 1);
        last_next->next = p2;
        update_last_match = (next_x < last_match_it->x_end);
    }

    // Link each component with the previous
    std::shared_ptr<path_point> pt = nullptr;
    while(_matching_bottom_lines.size() > 1) {
        bottom_line& match = *_matching_bottom_lines.back();
        if(pt != nullptr) {
            pt->next = match.start_point->next;
        }
        pt = match.start_point;
        // Close the component at the existing start/end (since we open it elsewhere to allow linking)
        std::shared_ptr<connected_component> component = _components[match.component_idx];
        component->path.end->next = component->path.start;
        // Remove the component by setting the pointer to null in the global list
        _components[match.component_idx] = nullptr;
        erased_component_idx.insert(match.component_idx);
        // Update the component for the current match
        match.component_idx = merged_component_idx;
        _matching_bottom_lines.pop_back();
    }

    // Update bottom lines that refer to components that were merged into the first one
    if(!erased_component_idx.empty()) {
        const auto update_if_needed = [&erased_component_idx, &merged_component_idx](bottom_line& match) {
            if(erased_component_idx.contains(match.component_idx)) {
                match.component_idx = merged_component_idx;
            }
        };
        // For the previous line, we only need to update matches after the last match we processed
        std::for_each(last_match_it + 1, _bottom_points.end(), update_if_needed);
        // For the current line, update all matches already created
        std::ranges::for_each(_next_points, update_if_needed);
    }

    // Close the path on the left + link the bottom
    ROL_CHECK(_matching_bottom_lines.size() == 1, "There should be exactly one match left after linking all components together");
    bottom_line& first_match = *_matching_bottom_lines.front();
    if(pt != nullptr) {
        // Link with the next content component
        pt->next = first_match.start_point->next;
        _components[first_match.component_idx]->path.is_simple = false;
    }
    if(x == first_match.x_start) {
        pt = first_match.start_point;
        pt->y++;
    } else {
        //              |               |
        // p1 O---------O---x--->---x---O
        //    |        start_point
        // pt O-------------------------->--------------O last_next
        _components[first_match.component_idx]->path.is_simple = false;
        std::shared_ptr<path_point> p1 = path_point::make(x, y);
        first_match.start_point->next = p1;
        pt = path_point::make(x, y+1);
        p1->next = pt;
    }
    pt->next = last_next;

    if(update_last_match) {
        // Since the last match might be needed by a later group, we must update its information
        last_match_it->start_point = last_next->next;
        last_match_it->x_start = last_match_it->start_point->x;
    }

    _next_points.push_back(bottom_line{ .component_idx = merged_component_idx, .start_point = pt, .x_start = x, .x_end = next_x });
}


void svg_generator::process(const rol::rgb_image& img) {
    process_first_line(img[0]);

    for(std::size_t y = 1; y < img.height(); y++) {
        std::swap(_bottom_points, _next_points);
        _next_points.clear();
        const std::span<const rol::rgba> row = img[y];
        bottom_iterator current_point_it = _bottom_points.begin();
        for(std::size_t x = 0; x < row.size();) {
            if(row[x].a == 0) {
                x++;
                continue;
            }
            const std::size_t next_x = get_next_colour_index(row, x);
            _matching_bottom_lines.clear();
            while(current_point_it != _bottom_points.end() && current_point_it->x_end <= x) {
                current_point_it++;
            }
            while(current_point_it != _bottom_points.end() && current_point_it->x_start < next_x) {
                if(row[x] == _components[current_point_it->component_idx]->colour) {
                    _matching_bottom_lines.push_back(current_point_it);
                }
                if(current_point_it->x_end <= next_x) {
                    current_point_it++;
                } else {
                    break;
                }
            }
            remove_multiple_line_matches();
            if(_matching_bottom_lines.empty()) {
                add_simple_component(row[x], y, x, next_x);
            } else {
                aggregate_with_matched_points(y, x, next_x);
            }
            x = next_x;
        }
    }
}

} // anonymous namespace


template <>
struct fmt::formatter<::polygonal_path> {
    constexpr auto parse(const fmt::format_parse_context& ctx) -> fmt::format_parse_context::iterator {
        if (ctx.begin() != ctx.end() && *ctx.begin() != '}') {
            throw fmt::format_error("invalid polygonal path format");
        }
        return ctx.begin();
    }

    auto format(const polygonal_path& path, fmt::format_context& ctx) const -> fmt::format_context::iterator {
        if(path.start == nullptr) {
            return ctx.out();
        }
        fmt::format_to(ctx.out(), "{},{}", path.start->x, path.start->y);
        const path_point* pt = path.start->next.get();
        while(pt != nullptr) {
            fmt::format_to(ctx.out(), " {},{}", pt->x, pt->y);
            pt = pt->next.get();
        }
        return ctx.out();
    }
};

template <>
struct fmt::formatter<::connected_component> {
    constexpr auto parse(const fmt::format_parse_context& ctx) -> fmt::format_parse_context::iterator {
        if (ctx.begin() != ctx.end() && *ctx.begin() != '}') {
            throw fmt::format_error("invalid connected component format");
        }
        return ctx.begin();
    }

    auto format(const connected_component& component, fmt::format_context& ctx) const -> fmt::format_context::iterator {
        if(component.path.is_simple) {
            const path_point& p1 = *component.path.start;
            const path_point& p2 = *p1.next;
            const path_point& p3 = *p2.next;
            return fmt::format_to(ctx.out(), R"__(<rect x="{}" y="{}" width="{}" height="{}" fill="{}"/>)__", p1.x, p1.y, p3.x - p2.x, p2.y - p1.y, component.colour);
        }
        return fmt::format_to(ctx.out(), R"__(<polygon points="{}" fill="{}"/>)__", component.path, component.colour);
    }
};


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
    ::svg_generator generator;
    generator.process(img);
    for(const std::shared_ptr<::connected_component>& component: generator.connected_components()) {
        if(component == nullptr) {
            continue;
        }
        fmt::format_to(format_it, "{}{}", indent, *component);
    }
    if(opts.pretty) {
        res.push_back('\n');
    }
    res.append("</svg>"sv);
    return res;
}

} // namespace rol::algo
