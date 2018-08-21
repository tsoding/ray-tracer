cmake_minimum_required(VERSION 3.9.2)
project(ray-tracer)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

find_package(SFML REQUIRED system window graphics)

include_directories(${SFML_INCLUDE_DIR})

set(RAY_TRACER_SOURCES
  src/color.cpp
  src/color.hpp
  src/main.cpp
  src/mat4x4.hpp
  src/plane.hpp
  src/scene.cpp
  src/scene.hpp
  src/sphere.hpp
  src/vec.hpp
  src/wall.hpp
  )

add_executable(ray-tracer ${RAY_TRACER_SOURCES})

set_target_properties(ray-tracer PROPERTIES
  CXX_STANDARD 17)

target_compile_options(ray-tracer PRIVATE
  -Wall
  -Werror
  -Wconversion
  -Wno-missing-braces
  -Wunused-variable
  -pedantic
  )

target_link_libraries(ray-tracer ${SFML_LIBRARIES})

include(CppLint)
add_cpp_lint_target(lint "${RAY_TRACER_SOURCES}")
add_dependencies(ray-tracer lint)
