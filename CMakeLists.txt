cmake_minimum_required(VERSION 3.9.2)
project(ray-tracer)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

find_package(SFML REQUIRED system window graphics)
find_package(Threads REQUIRED)

include_directories(${SFML_INCLUDE_DIR})

set(RAY_TRACER_SOURCES
  src/arguments.cpp
  src/arguments.hpp
  src/color.cpp
  src/color.hpp
  src/display.cpp
  src/display.hpp
  src/main.cpp
  src/scene.cpp
  src/scene.hpp
  src/sphere.cpp
  src/sphere.hpp
  src/triangle.cpp
  src/triangle.hpp
  src/vec.hpp
  src/wall.cpp
  src/wall.hpp
  src/ray.hpp
  src/ray.cpp
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
