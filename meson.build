project('minecraft_clone', 'c','cpp', version : '1.0.0', default_options : ['cpp_std=c++17'])

# cpp = meson.get_compiler('cpp')

src = [
  './lib/glad/src/glad.c',
  './lib/imgui/imgui.cpp',
  './lib/imgui/imgui_tables.cpp',
  './lib/imgui/imgui_draw.cpp',
  './lib/imgui/imgui_widgets.cpp',
  './lib/imgui/imgui_impl_glfw.cpp',
  './lib/imgui/imgui_impl_opengl3.cpp',
  
  './src/gfx/window.cpp',
  './src/gfx/texture_manager.cpp',
  './src/util/util.cpp',
  './src/util/gui.cpp',
  './src/world/chunk.cpp',
  './src/world/world.cpp',  
  './src/main.cpp'
]

glfw_dep = dependency('glfw3')

libs = include_directories('lib', 'lib/glad/include')

app_target = executable('App',
  sources : src,
  include_directories: libs,
  dependencies : glfw_dep,
  cpp_args : ['-std=c++17']
)


