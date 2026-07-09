
rossignol_demo_names = []

all_demos = []

foreach name: rossignol_demo_names
    all_demos += executable(name,
        '@0@.cc'.format(name),
        dependencies: [rossignol_dep],
        cpp_args: cpp_warning_flags,
        install: false
    )
endforeach

alias_target('rossignol_demos', all_demos)
