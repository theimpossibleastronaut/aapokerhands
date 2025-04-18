src_aapokerhands = ['aapokerhands.c']

deckhandler_dep = dependency(
  'deckhandler',
  version : '>=0.1.0',
  fallback : ['deckhandler', 'deckhandler_dep'], default_options: 'default_library=shared',
  )

protobuf_dep = dependency('libprotobuf-c')

if get_option('gen_protobuf')
  protoc = find_program('protoc', required: true)
  run_command([protoc, '--c_out=.', 'netpoker.proto'], check: true)
endif

executable(
  'aapokerhands',
  src_aapokerhands,
  dependencies: deckhandler_dep,
  install:true
  )

executable(
  'netpoker',
  ['netpoker.c', 'net.c', 'netpoker.pb-c.c'],
  dependencies: [deckhandler_dep, protobuf_dep],
  install:true
  )

conf = configuration_data()
conf.set_quoted('VERSION', meson.project_version())
conf.set_quoted('PACKAGE_STRING', meson.project_name())
conf.set_quoted('PACKAGE_URL', 'https://github.com/theimpossibleastronaut/aapokerhands')
config_h = configure_file(output : 'config.h', configuration : conf)
