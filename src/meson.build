src_aapokerhands = ['aapokerhands.c']

deckhandler_dep = dependency(
  'deckhandler',
  version: '>=0.1.0',
  fallback: ['deckhandler', 'deckhandler_dep'],
  default_options: 'default_library=shared',
)

protobuf_dep = dependency('libprotobuf-c')

if get_option('gen_protobuf')
  protoc = find_program('protoc', required: true)
  run_command([protoc, '--c_out=./protobuf', 'netpoker.proto'], check: true)
endif

_lib = static_library(
  'lib',
  ['lib.c'],
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

foreach bin : net_binaries
  executable(
    bin,
    [bin + '.c', 'game.c', 'net.c', 'protobuf/netpoker.pb-c.c'],
    dependencies: [deckhandler_dep, protobuf_dep],
    link_with: _lib,
    include_directories: include_directories('protobuf'),
    install: false,
  )
endforeach




conf = configuration_data()
conf.set_quoted('VERSION', meson.project_version())
conf.set_quoted('PACKAGE_STRING', meson.project_name())
conf.set_quoted(
  'PACKAGE_URL',
  'https://github.com/theimpossibleastronaut/aapokerhands',
)
config_h = configure_file(output: 'config.h', configuration: conf)
