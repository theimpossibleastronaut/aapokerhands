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

definitions = ['-D_DEFAULT_SOURCE']
add_project_arguments(definitions, language: 'c')

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

subdir('src')

install_data(
  files('COPYING', 'ChangeLog', 'README.md', 'example_output01.txt'),
  install_dir: get_option('docdir'),
)
