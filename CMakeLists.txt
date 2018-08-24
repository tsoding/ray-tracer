cmake_minimum_required(VERSION 3.9.2)
project(ray-tracer)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

find_package(SFML REQUIRED system window graphics)

include_directories(${SFML_INCLUDE_DIR})

set(RAY_TRACER_SOURCES
  src/main.cpp)

set(RAY_TRACER_HEADERS
  color.hpp
  mat4x4.hpp
  vec.hpp)

set(GLAD_SOURCES
	src/glad.c)

add_executable(ray-tracer ${RAY_TRACER_SOURCES})
add_library(glad STATIC ${GLAD_SOURCES})

set_target_properties(ray-tracer PROPERTIES
  CXX_STANDARD 17)

set_target_properties(glad PROPERTIES
  C_STANDARD 90)

target_compile_options(ray-tracer PRIVATE
  -Wall
  -Werror
  -Wconversion
  -Wno-missing-braces
  -Wunused-variable
  -pedantic)

target_link_libraries(ray-tracer PRIVATE ${SFML_LIBRARIES} GL glad dl)
target_include_directories(ray-tracer PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
target_include_directories(glad PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)

configure_file(src/shader.vert shader.vert COPYONLY)
configure_file(src/shader.frag shader.frag COPYONLY)
