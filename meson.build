project(
  'pyclipper2',
  'cpp',
  version: '0.0.8',
  meson_version: '>=1.0.0',
  default_options: ['cpp_std=c++23', 'b_ndebug=if-release'],
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
  'pyclipper2',
  sources: ['src/bindings.cpp', clipper2_sources],
  dependencies: [nanobind_dep],
  include_directories: clipper2_inc,
  install: true,
)

### this part generates the type stubs using nanobind's stubgen tool
### I can't get this to work but I will keep it here for when I want to re-visit it

# stubgen = nanobind_dep.get_variable('stubgen')
# custom_target(
#   output: 'pyclipper2.pyi',
#   depends: mod,
#   command: [python, stubgen, '-m', 'pyclipper2', '-M', 'py.typed'],
#   build_by_default: true
# )
