src = ['aa-pokerhands.c', 'functions.c']
dep = 'deckhandler'
dep_target = static_library(dep, join_paths(dep, dep + '.c'))

extra_args = ['-fno-common']
executable('pokerhands', src, c_args : extra_args, link_with : dep_target, dependencies : dependency('threads'))

conf = configuration_data()
conf.set('VERSION', meson.project_version())
conf.set_quoted('PACKAGE_STRING', meson.project_name())
config_h = configure_file(output : 'config.h', configuration : conf)
