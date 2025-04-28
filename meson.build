project(
  'aapokerhands',
  'c',
  version: '0.1.1.555',
  meson_version: '>= 0.48.0',
  default_options: [
    'warning_level=3',
    'b_sanitize=address,undefined',
    'b_lundef=false',
    'c_std=c99',
  ],
)

cc = meson.get_compiler('c')

extra_flags = [
  '-fno-common',
  '-fstack-protector-strong',
  '-Wformat-security',
  '-Wshadow',
  '-Wstrict-overflow=5',
  '-Werror=odr',
  '-Werror=lto-type-mismatch',
  '-Werror=strict-aliasing',
]

add_project_arguments(cc.get_supported_arguments(extra_flags), language: 'c')

pokeval_dep = dependency(
  'pokeval',
  default_options: 'default_library=static',
)

aapokerhands_bin = executable(
  'aapokerhands',
  'aapokerhands.c',
  dependencies: pokeval_dep,
  install: true,
)

test('aapokerhands_bin_test', aapokerhands_bin, args: ['-n', '100000'])

conf = configuration_data()
conf.set_quoted('VERSION', meson.project_version())
conf.set_quoted('PACKAGE_STRING', meson.project_name())
conf.set_quoted(
  'PACKAGE_URL',
  'https://github.com/theimpossibleastronaut/aapokerhands',
)
config_h = configure_file(output: 'config.h', configuration: conf)

install_data(
  files(
    'AUTHORS',
    'COPYING',
    'ChangeLog',
    'README.md',
  ),
  install_dir: get_option('docdir'),
)
