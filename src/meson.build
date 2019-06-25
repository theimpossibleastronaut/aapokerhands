src = ['aa-pokerhands.c', 'functions.c']
dep = 'deckhandler'
dep_target = static_library(dep, join_paths(dep, dep + '.c'))
executable('pokerhands', src, link_with : dep_target)

conf = configuration_data()
conf.set('VERSION', meson.project_version())
conf.set_quoted('PACKAGE_STRING', meson.project_name())
config_h = configure_file(output : 'config.h', configuration : conf)
