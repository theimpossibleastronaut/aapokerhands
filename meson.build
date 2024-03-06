project(
  'aapokerhands',
  'c',
  version: '0.1.1.555',
  meson_version : '>= 0.48.0',
  default_options: [
    'warning_level=3',
    'b_sanitize=address,undefined',
    'b_lundef=false'
    ]
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
