cc = meson.get_compiler('c')
src = ['aapokerhands.c']
extra_args = [
  '-fno-common',
  '-Wshadow'
]
add_project_arguments(cc.get_supported_arguments(extra_args), language: 'c')

deckhandler_dep = dependency(
  'deckhandler',
  version : '>=0.1.0',
  fallback : ['deckhandler', 'deckhandler_dep'], default_options: 'default_library=shared',
  )

executable(
  'aapokerhands',
  src,
  dependencies: deckhandler_dep,
  install:true
  )

conf = configuration_data()
conf.set_quoted('VERSION', meson.project_version())
conf.set_quoted('PACKAGE_STRING', meson.project_name())
conf.set_quoted('PACKAGE_URL', 'https://github.com/theimpossibleastronaut/aapokerhands')
config_h = configure_file(output : 'config.h', configuration : conf)
