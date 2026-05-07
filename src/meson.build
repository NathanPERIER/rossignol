
rossignol_sources = [
    'algorithm/binarise.cc',
    'algorithm/coefficients.cc',
    'algorithm/extract_channel.cc',
    'algorithm/greyscale.cc',
    'colour/rgb.cc',
    'generic/binarise.cc',
    'generic/coefficients.cc',
    'generic/extract_channel.cc',
    'generic/greyscale.cc',
    'generic/mirror.cc',
    'image/image.cc',
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
