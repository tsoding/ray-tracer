cmake_minimum_required(VERSION 3.10)
project(ray-tracer)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

find_package(SFML REQUIRED system window graphics)

include_directories(${SFML_INCLUDE_DIR})

# TODO: cpplint is not performed in cmake build

set(CMAKE_CXX_FLAGS
  "${CMAKE_CXX_FLAGS} \
   -Wall \
   -Werror \
   -Wconversion \
   -Wno-missing-braces \
   -Wunused-variable \
   -pedantic \
   -std=c++17")

add_executable(ray-tracer src/main.cpp)
target_link_libraries(ray-tracer ${SFML_LIBRARIES})
