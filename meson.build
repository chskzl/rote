project(
  'rote',
  'c',
  default_options: [
    'c_std=gnu11',
  ],
)

gtk_dep = dependency('gtk4')

executable(
  'rote',
  [
    'src/app.c',
    'src/main.c',
  ],
  dependencies: [gtk_dep],
  include_directories: include_directories('src/include'),
)

