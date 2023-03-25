project(
  'aapokerhands',
  'c',
  version: '0.1.0.555',
  meson_version : '>= 0.48.0',
  default_options: 'warning_level=2'
)
subdir('src')

install_data(
  files(
    'README.md',
    'COPYING',
    'ChangeLog',
    'example_output01.txt'
    ),
  install_dir : get_option('docdir')
  )
