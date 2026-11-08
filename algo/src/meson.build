
rossignol_algo_sources = [
    'algorithm/binarise.cc',
    'algorithm/channels.cc',
    'algorithm/coefficients.cc',
    'algorithm/colourmap.cc',
    'algorithm/filters.cc',
    'algorithm/greyscale.cc',
    'algorithm/overflow.cc',
    'algorithm/rgb.cc',
    'algorithm/generators/random.cc'
]

rossignol_algo_deps = [rossignol_base_dep]

rossignol_algo_lib = static_library(
    'rossignol_algo_static', rossignol_algo_sources,
    dependencies: rossignol_algo_deps,
    cpp_args: cpp_warning_flags,
    link_args: [],
    include_directories: rossignol_algo_headers_dir,
    install: false,
    build_by_default: rossignol_build_default
)

rossignol_algo_dep = declare_dependency(
    link_with : rossignol_algo_lib,
    dependencies: rossignol_algo_deps,
    include_directories : rossignol_algo_headers_dir
)
