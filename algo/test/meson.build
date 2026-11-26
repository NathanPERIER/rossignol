
algo_test_sources = [
    'svg.cc'
]


foreach main_path : algo_test_sources
    test_name = main_path.substring(0, -3).replace('/', '-') + '-test'
    test_exe = executable(
        test_name, [main_path],
        dependencies: [rossignol_algo_dep, catch2_dep],
        install: false,
        build_by_default: not meson.is_subproject()
    )
    test(test_name, test_exe)
endforeach
