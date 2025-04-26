# Minimum required CMake version
cmake_minimum_required(VERSION 3.5.0)

# Project name, version, and languages (C and C++)
project(hellow_window VERSION 1.0.0 LANGUAGES CXX C)

set(OpenGL_GL_PREFERENCE GLVND)  # Use GLVND over legacy

# Find OpenGL
find_package(OpenGL 3.3 REQUIRED)

# Add executable
add_executable(hello_window 
	src/config.h 
	src/main.cpp 
	src/glad.c
	src/triangle_mesh.cpp
)

# Add dependencies
target_include_directories(hello_window 
	PRIVATE 
	dependencies
)

# Link libraries
target_link_libraries(hello_window 
	/opt/homebrew/Cellar/glfw/3.4/lib/libglfw.3.dylib 
	OpenGL::GL
)
