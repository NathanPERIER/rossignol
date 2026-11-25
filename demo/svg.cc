
#include <deque>

#include <spdlog/spdlog.h>

#include <rossignol/algorithm/svg.hh>
#include <rossignol/image/io.hh>
#include <rossignol/utils/io/file.hh>
#include <rossignol/utils/logging/init.hh>


void print_usage(std::string_view prog_name, bool ok) {
    const spdlog::level::level_enum level = ok ? spdlog::level::level_enum::info : spdlog::level::level_enum::err;
    spdlog::log(level, "usage: {} -i <input> [--html] [-o <output>]", prog_name);
}


std::string html_wrap(std::string svg) {
    std::string res = R"__(<!doctype html>
<html lang="en-GB">
    <head> 
        <meta charset="utf-8"> 
        <title>SVG test</title>
        <style>
            rect:hover {
                fill: url(#grad-rect);
            }
            polygon:hover {
                fill: url(#grad-polygon);
            }
            path:hover {
                fill: url(#grad-path);
            }
        </style>
    </head>
    <body>
        <svg xmlns="http://www.w3.org/2000/svg" height="0" width="0">
            <defs>
                <linearGradient id="grad-rect" x1="0" y1="0" x2="1" y2="1" spreadMethod="repeat" gradientUnits="userSpaceOnUse">
                    <stop offset="50%" stop-color="#26e09f"/>
                    <stop offset="50%" stop-color="#177b58"/>
                </linearGradient>
                <linearGradient id="grad-polygon" x1="0" y1="0" x2="1" y2="1" spreadMethod="repeat" gradientUnits="userSpaceOnUse">
                    <stop offset="50%" stop-color="#26a9e0"/>
                    <stop offset="50%" stop-color="#186586"/>
                </linearGradient>
                <linearGradient id="grad-path" x1="0" y1="0" x2="1" y2="1" spreadMethod="repeat" gradientUnits="userSpaceOnUse">
                    <stop offset="50%" stop-color="#7326e0"/>
                    <stop offset="50%" stop-color="#2e0f59"/>
                </linearGradient>
            </defs>
        </svg>
)__";
    res.append(svg);
    res.append("\n    </body>\n</html>");
    return res;
}


void process(std::filesystem::path input_file, bool html, std::optional<std::filesystem::path> output_file) {
    const rol::rgb_image sprite = rol::load_image_rgba(input_file);

    std::string svg = rol::algo::to_svg(sprite, rol::params::svg_options {
        .xml_header = !html,
        .bounding_box = false,
        .pretty = true
    });

    if(html) {
        svg = html_wrap(std::move(svg));
    }

    if(output_file.has_value()) {
        rol::write_to_file(output_file.value(), svg);
    } else {
        fmt::println("{}", svg);
    }
}


int main(int argc, char** argv) {
    rol::init_logger();
    const std::string_view prog_name = (argc > 0) ? *argv : "svg";

    std::deque<std::string_view> args;
    for(int i = 1; i < argc; i++) {
        args.push_back(*(argv+i));
    }

    if(!args.empty() && (args.front() == "-h" || args.front() == "--help")) {
        print_usage(prog_name, true);
        return 0;
    }

    if(args.size() < 2 || args.front() != "-i") {
        print_usage(prog_name, false);
        return 1;
    }
    args.pop_front();
    const std::filesystem::path input_file(args.front());
    args.pop_front();

    bool html = false;
    if(args.size() >= 1 && args.front() == "--html") {
        args.pop_front();
        html = true;
    }

    std::optional<std::filesystem::path> output_file;
    if(args.size() >= 2 && args.front() == "-o") {
        args.pop_front();
        output_file = args.front();
        args.pop_front();
    }

    if(!args.empty()) {
        print_usage(prog_name, false);
        return 1;
    }

    process(input_file, html, output_file);

    return 0;
}
