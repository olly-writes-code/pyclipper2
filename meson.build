project(
  'pyclipper2',
  'cpp',
  version: '0.0.1',
  meson_version: '>=1.0.0',
  default_options: ['cpp_std=c++17', 'b_ndebug=if-release'],
)

clipper2_inc = include_directories('external/clipper2/CPP/Clipper2Lib/include')

clipper2_sources = files(
  'external/clipper2/CPP/Clipper2Lib/src/clipper.engine.cpp',
  'external/clipper2/CPP/Clipper2Lib/src/clipper.offset.cpp',
  'external/clipper2/CPP/Clipper2Lib/src/clipper.rectclip.cpp',
)

python = import('python').find_installation()
nanobind_dep = dependency('nanobind')
mod = python.extension_module(
  'my_module_name',
  sources: ['pyclipper2/bindings.cpp', clipper2_sources],
  dependencies: [nanobind_dep],
  include_directories: clipper2_inc,
  install: true,
)
