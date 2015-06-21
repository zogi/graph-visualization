cmake_minimum_required(VERSION 2.6)
project(graph)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})

file(GLOB HEADERS include/*.h include/GL/*.h)
file(GLOB SOURCES src/*.cpp src/*.c)

include_directories(include)

add_executable(graph ${HEADERS} ${SOURCES})
target_link_libraries(graph ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} ${GLUT_glut_LIBRARY})
