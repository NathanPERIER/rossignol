project('rossignol', ['c','cpp'], default_options: ['cpp_std=c++2a'])

cmake = import('cmake')

cpp_warning_flags = [
  '-Wall',
  '-Wextra',
  '-pedantic-errors',
  '-Wdisabled-optimization',
  '-Wmisleading-indentation',
  '-Werror=unused-result',
  '-Werror=return-type',
  '-Werror=empty-body'
]

fmt_proj = cmake.subproject('fmt')
fmt_dep = fmt_proj.dependency('fmt')

png_dep = dependency('libpng')


subdir('include')
subdir('src')
