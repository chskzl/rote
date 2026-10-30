project(
  'rote',
  'c',
  default_options: [
    'c_std=gnu11',
  ],
)

gtk_dep = dependency('gtk4')
layer_shell_dep = dependency('gtk4-layer-shell-0')

executable(
  'rote',
  [
    'src/app.c',
    'src/main.c',
  ],
  dependencies: [gtk_dep, layer_shell_dep],
  include_directories: include_directories('src/include'),
)

