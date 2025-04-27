# Minimum required CMake version
cmake_minimum_required(VERSION 3.5.0)

# Project name, version, and languages (C and C++)
project(hellow_window VERSION 1.0.0 LANGUAGES CXX C)

cmake_policy(SET CMP0072 NEW)

set(CMAKE_CXX_STANDARD 20)

# set(OpenGL_GL_PREFERENCE GLVND)  # Use GLVND over legacy

# Find OpenGL
find_package(OpenGL REQUIRED)

# Add executable
add_executable(hello_window 
	src/config.h 
	src/config.cpp
	src/main.cpp 
	src/glad.c
	src/factories/factory.cpp
    src/systems/camera_system.cpp
    src/systems/motion_system.cpp
    src/systems/render_system.cpp
    src/view/shader.cpp
    src/controller/app.cpp
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
