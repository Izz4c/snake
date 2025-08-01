cmake_minimum_required(VERSION 3.10)
project(SnakeGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(snake src/main.cpp)

# Find and link OpenGL
find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)
pkg_check_modules(GLEW REQUIRED glew)

include_directories(
	${GLFW_INCLUDE_DIRS}
	${GLEW_INCLUDE_DIRS}
)

target_link_libraries(snake
	${OPENGL_gl_LIBRARY}
	${GLFW_LIBRARIES}
	${GLEW_LIBRARIES}
)

