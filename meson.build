project('kingdbengine', 'cpp', 
  version : '0.1', 
  meson_version : '>= 0.55.3',
  default_options : ['warning_level=3',
                     'cpp_std=c++17'])

subdir('Basic')
subdir('Modules')
subdir('Platform')
subdir('tests')

# create shared library - kingdbengine
# The game will use the engine by link the library
