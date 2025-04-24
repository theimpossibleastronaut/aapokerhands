exe = executable(
  'test_lib',
  '../src/lib.c',
  c_args: ['-DTEST_LIB'],
  dependencies: deckhandler_dep,
  link_with: _lib,
  override_options: ['b_lto=false'],
)
test('test_lib', exe)
