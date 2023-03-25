cc = meson.get_compiler('c')
src = ['aapokerhands.c']
extra_args = [
  '-fno-common',
  '-Wshadow'
]
add_project_arguments(cc.get_supported_arguments(extra_args), language: 'c')

dep = 'deckhandler'
dep_target = static_library(dep, join_paths(dep, dep + '.c'))

executable('aapokerhands', src, link_with : dep_target, install:true)

conf = configuration_data()
conf.set('VERSION', meson.project_version())
conf.set_quoted('PACKAGE_STRING', meson.project_name())
config_h = configure_file(output : 'config.h', configuration : conf)
