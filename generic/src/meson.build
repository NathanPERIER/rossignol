
rossignol_generic_sources = [
    'colour/generic.cc',
    'generic/binarise.cc',
    'generic/bit_planes.cc',
    'generic/channels.cc',
    'generic/coefficients.cc',
    'generic/colourmap.cc',
    'generic/fill.cc',
    'generic/filters.cc',
    'generic/greyscale.cc',
    'generic/invert.cc',
    'generic/mirror.cc',
    'generic/prepare_write.cc',
    'generic/rgb.cc',
    'generic/sinks/file.cc',
    'params/generic/offset.cc',
    'params/generic/size.cc'
]

rossignol_generic_deps = [rossignol_algo_dep]

rossignol_generic_lib = static_library(
    'rossignol_generic_static', rossignol_generic_sources,
    dependencies: rossignol_generic_deps,
    cpp_args: cpp_warning_flags,
    link_args: [],
    include_directories: rossignol_generic_headers_dir,
    install: false,
    build_by_default: true
)

rossignol_generic_dep = declare_dependency(
    link_with : rossignol_generic_lib,
    dependencies: rossignol_generic_deps,
    include_directories : rossignol_generic_headers_dir
)
