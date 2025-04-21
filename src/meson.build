src_aapokerhands = ['aapokerhands.c']

deckhandler_dep = dependency('deckhandler', default_options: 'default_library=shared',
)

sdl_dep = dependency('SDL2')
sdl_ttf_dep = dependency('SDL2_ttf')
protobuf_dep = dependency('libprotobuf-c')

if get_option('gen_protobuf')
  protoc = find_program('protoc', required: true)
  run_command([protoc, '--c_out=./protobuf', 'netpoker.proto'], check: true)
endif

lib_src = files('lib.c')

_lib = static_library(
  'lib',
  lib_src,
  dependencies: deckhandler_dep,
  install: false,
)

executable(
  'aapokerhands',
  src_aapokerhands,
  dependencies: deckhandler_dep,
  link_with: _lib,
  install: true,
)

net_binaries = ['netpoker_server', 'netpoker_client']
net_shared_src = ['game.c', 'net.c', 'netpoker.c', 'protobuf/netpoker.pb-c.c']
net_shared_dep = [deckhandler_dep, protobuf_dep, win_deps]
net_shared_inc = include_directories('protobuf')

executable(
  'netpoker_server',
  ['netpoker_server.c', net_shared_src],
  dependencies: net_shared_dep,
  link_with: _lib,
  include_directories: net_shared_inc,
  install: false,
)

executable(
  'netpoker_client',
  ['netpoker_client.c', net_shared_src, 'graphics.c'],
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
