project(
  'aapokerhands',
  'c',
  version: '0.1.1.555',
  meson_version: '>= 0.51.0',
  default_options: [
    'warning_level=3',
    'b_sanitize=address,undefined',
    'b_lundef=false',
    'c_std=c99',
  ],
)

cc = meson.get_compiler('c')

host_sys = host_machine.system()
platform_is_windows = (host_sys == 'windows')

win_deps = []
if platform_is_windows == true
    win_deps += cc.find_library('ws2_32')
    win_deps += cc.find_library('winpthread', static: true)
endif

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

if host_sys == 'linux'
  subdir('tests')
endif

install_data(
  files('COPYING', 'ChangeLog', 'README.md', 'example_output01.txt'),
  install_dir: get_option('docdir'),
)
