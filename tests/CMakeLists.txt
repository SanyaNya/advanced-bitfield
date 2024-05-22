cmake_minimum_required(VERSION 3.16)
project(advanced-bitfield-tests)

include(FetchDoctest.cmake)

add_executable(tests tests_main.cpp
  tests/simple.cpp
  tests/overlapping_parts.cpp
  tests/x86_segment_descriptor.cpp)
target_link_libraries(tests PRIVATE doctest::doctest)
target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
