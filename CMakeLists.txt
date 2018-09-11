cmake_minimum_required(VERSION 3.9.2)
project(ray-tracer)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

find_package(SFML REQUIRED system window graphics)
find_package(Threads REQUIRED)

include_directories(${SFML_INCLUDE_DIR})

set(RAY_TRACER_SOURCES
  src/color.cpp
  src/color.hpp
  src/display.cpp
  src/display.hpp
  src/main.cpp
  src/rand_rendering_scene.hpp
  src/rendering_scene.hpp
  src/scene.cpp
  src/scene.hpp
  src/seq_rendering_scene.hpp
  src/sphere.cpp
  src/sphere.hpp
  src/sprite_display.cpp
  src/sprite_display.hpp
  src/texture_display.cpp
  src/texture_display.hpp
  src/triangle.cpp
  src/triangle.hpp
  src/vec.hpp
  src/wall.cpp
  src/wall.hpp
  )

add_executable(ray-tracer ${RAY_TRACER_SOURCES})

set_target_properties(ray-tracer PROPERTIES
  CXX_STANDARD 14)

target_compile_options(ray-tracer PRIVATE
  -Wall
  -Werror
  -Wconversion
  -Wno-missing-braces
  -Wunused-variable
  -pedantic
  )

target_link_libraries(ray-tracer ${SFML_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

include(CppLint)
add_cpp_lint_target(lint "${RAY_TRACER_SOURCES}")
add_dependencies(ray-tracer lint)
