
rossignol_sources = [
    'algorithm/binarise.cc',
    'algorithm/channels.cc',
    'algorithm/coefficients.cc',
    'algorithm/colourmap.cc',
    'algorithm/greyscale.cc',
    'algorithm/rgb.cc',
    'colour/colourmap.cc',
    'colour/generic.cc',
    'colour/conv/cylindrical.cc',
    'generic/binarise.cc',
    'generic/channels.cc',
    'generic/coefficients.cc',
    'generic/colourmap.cc',
    'generic/fill.cc',
    'generic/greyscale.cc',
    'generic/invert.cc',
    'generic/mirror.cc',
    'generic/rgb.cc',
    'image/io.cc',
    'params/generic/offset.cc',
    'params/generic/size.cc',
    'utils/io/file.cc'
]

rossignol_deps = [fmt_dep, png_dep]

rossignol_lib = static_library(
    'rossignol_static', rossignol_sources,
    dependencies: rossignol_deps,
    cpp_args: cpp_warning_flags,
    link_args: [],
    include_directories: rossignol_headers_dir,
    install: false,
    build_by_default: true
)

rossignol_dep = declare_dependency(
    link_with : rossignol_lib,
    dependencies: rossignol_deps,
    include_directories : rossignol_headers_dir
)
