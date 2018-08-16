cmake_minimum_required(VERSION 3.9.2)
project(ray-tracer)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

find_package(SFML REQUIRED system window graphics)

include_directories(${SFML_INCLUDE_DIR})

# TODO(#51): cpplint is not performed in cmake build

add_executable(ray-tracer src/main.cpp)

set_target_properties(ray-tracer PROPERTIES
  CXX_STANDARD 17)

target_compile_options(ray-tracer PRIVATE
  -Wall
  -Werror
  -Wconversion
  -Wno-missing-braces
  -Wunused-variable
  -pedantic)

target_link_libraries(ray-tracer ${SFML_LIBRARIES})
