src_aapokerhands = ['aapokerhands.c']

deckhandler_dep = dependency(
  'deckhandler',
  version : '>=0.1.0',
  fallback : ['deckhandler', 'deckhandler_dep'], default_options: 'default_library=shared',
  )

executable(
  'aapokerhands',
  src_aapokerhands,
  dependencies: deckhandler_dep,
  install:true
  )

executable(
  'netpoker',
  ['netpoker.c', 'net.c'],
  dependencies: deckhandler_dep,
  install:true
  )

conf = configuration_data()
conf.set_quoted('VERSION', meson.project_version())
conf.set_quoted('PACKAGE_STRING', meson.project_name())
conf.set_quoted('PACKAGE_URL', 'https://github.com/theimpossibleastronaut/aapokerhands')
config_h = configure_file(output : 'config.h', configuration : conf)
