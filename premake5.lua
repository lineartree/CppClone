workspace "minecraft_clone"
  location "build/makefiles"
  architecture "x86_64"

  configurations {"Debug", "Release"}

  filter {"configurations:Debug"}
    symbols "On"
    buildoptions {"-Wall -g -fopenmp"}
  
  filter {"configurations:Release"}
    optimize "On"
    buildoptions {"-O2 -fopenmp"}

  filter {}

  targetdir ("build/bin/%{prj.name}/%{cfg.longname}")
  objdir ("build/obj/%{prj.name}/%{cfg.longname}")

project "App"
  kind "ConsoleApp"
  cppdialect "C++17"
  files {
    "./src/*.cpp",
    "./src/gfx/*.cpp",
    "./src/util/*.cpp",
    "./src/world/*.cpp",
    "./lib/glad/glad.c",
    "./lib/imgui/*.cpp"
  }
	removefiles {"./lib/imgui/imgui_demo.cpp"}

  includedirs {"./lib"}
  links {"GL", "dl", "glfw", "gomp"}
  -- links {"GL", "dl", "glfw", "GLEW", "gomp"}
