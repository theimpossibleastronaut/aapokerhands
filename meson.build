project(
  'aapokerhands',
  'c',
  version: '0.1.1.555',
  meson_version: '>= 0.51.0',
  default_options: [
    'warning_level=3',
    'b_sanitize=address,undefined',
    'b_lundef=false',
    'c_std=c11',
  ],
)

cc = meson.get_compiler('c')

host_sys = host_machine.system()
platform_is_windows = (host_sys == 'windows')

win_deps = []
if platform_is_windows == true
    win_deps += cc.find_library('ws2_32')
    win_deps += cc.find_library('winpthread', static: true)
endif

definitions = ['-D_DEFAULT_SOURCE']
add_project_arguments(definitions, language: 'c')

extra_flags = [
  '-fno-common',
  '-fstack-protector-strong',
  '-Wformat-security',
  '-Wshadow',
  '-Wstrict-overflow=5',
  '-Werror=odr',
  '-Werror=lto-type-mismatch',
  '-Werror=strict-aliasing',
  #'-Wconversion',
]

add_project_arguments(cc.get_supported_arguments(extra_flags), language: 'c')

deckhandler_dep = dependency('deckhandler', default_options: 'default_library=shared',
)

sdl_dep = dependency('SDL2')
sdl_ttf_dep = dependency('SDL2_ttf')
protobuf_dep = dependency('libprotobuf-c')

net_shared_dep = [deckhandler_dep, protobuf_dep, win_deps]
net_shared_inc = include_directories('src/protobuf')

subdir('src')

_lib = static_library(
  'lib',
  lib_src,
  dependencies: deckhandler_dep,
  install: false,
)

executable(
  'aapokerhands',
  'src/aapokerhands.c',
  dependencies: deckhandler_dep,
  link_with: _lib,
  install: true,
)

#net_binaries = ['netpoker_server', 'netpoker_client']

executable(
  'netpoker_server',
  ['src/netpoker_server.c', net_shared_src],
  dependencies: net_shared_dep,
  link_with: _lib,
  include_directories: net_shared_inc,
  install: false,
)

executable(
  'netpoker_client',
  ['src/netpoker_client.c', net_shared_src, 'src/graphics.c'],
  dependencies: [net_shared_dep, sdl_dep, sdl_ttf_dep],
  link_with: _lib,
  include_directories: net_shared_inc,
  install: false,
)

conf = configuration_data()
conf.set_quoted('VERSION', meson.project_version())
conf.set_quoted('PACKAGE_STRING', meson.project_name())
conf.set_quoted(
  'PACKAGE_URL',
  'https://github.com/theimpossibleastronaut/aapokerhands',
)
config_h = configure_file(output: 'config.h', configuration: conf)


if host_sys == 'linux'
  subdir('tests')
endif

install_data(
  files('COPYING', 'ChangeLog', 'README.md', 'example_output01.txt'),
  install_dir: get_option('docdir'),
)
