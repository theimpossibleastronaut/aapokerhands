src = ['aa-pokerhands.c', 'functions.c', 'deckhandler/deckhandler.c']
executable('pokerhands', src,
  install: true)

conf = configuration_data()
conf.set('VERSION', meson.project_version())
conf.set_quoted('PACKAGE_STRING', meson.project_name())

config_h = configure_file(
        output : 'config.h',
configuration : conf)
