cmake_minimum_required(VERSION 3.20)
project(gengine VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add Glad (C file)
add_library(glad glad/src/glad.c)
target_include_directories(glad PUBLIC glad/include)

# Find Dependencies
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

# Define Executable
add_executable(gengine-bin src/main.cc)

# Link everything
target_link_libraries(gengine-bin PRIVATE glad glfw OpenGL::GL)
target_include_directories(gengine-bin PRIVATE include)