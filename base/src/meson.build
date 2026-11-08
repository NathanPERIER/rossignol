
rossignol_base_sources = [
    'colour/colourmap.cc',
    'colour/conv/cylindrical.cc',
    'image/io.cc',
    'utils/io/file.cc'
]

rossignol_base_deps = [fmt_dep, png_dep]

rossignol_base_lib = static_library(
    'rossignol_base_static', rossignol_base_sources,
    dependencies: rossignol_base_deps,
    cpp_args: cpp_warning_flags,
    link_args: [],
    include_directories: rossignol_base_headers_dir,
    install: false,
    build_by_default: rossignol_build_default
)

rossignol_base_dep = declare_dependency(
    link_with : rossignol_base_lib,
    dependencies: rossignol_base_deps,
    include_directories : rossignol_base_headers_dir
)
