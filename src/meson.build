
rossignol_sources = [
    'colour/rgb.cc',
    'image/image.cc',
    'image/algorithm/binarise.cc',
    'image/algorithm/greyscale.cc',
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
