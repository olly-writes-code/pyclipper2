project(
  'my_project_name',
  'cpp',
  version: '0.0.1',
  meson_version: '>=1.0.0',
  default_options: ['cpp_std=c++17', 'b_ndebug=if-release'],
)

python = import('python').find_installation()
nanobind_dep = dependency('nanobind')
mod = python.extension_module(
  'my_module_name',
  sources: ['path_to_module.cpp'],
  dependencies: [nanobind_dep],
  install: true,
)
