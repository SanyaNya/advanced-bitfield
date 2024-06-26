cmake_minimum_required(VERSION 3.16)
project(advanced-bitfield-tests)

include(FetchDoctest.cmake)

add_executable(${PROJECT_NAME} tests_main.cpp
  tests/simple.cpp
  tests/overlapping_parts.cpp
  tests/x86_segment_descriptor.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE doctest::doctest)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_compile_options(${PROJECT_NAME} PRIVATE
  -Wall;
  -Wextra;
  -Wconversion;
  -Wsign-conversion;
  -pedantic;
  -O0;
  -g3;
  -fsanitize=address;
  -fsanitize=undefined)

target_link_options(${PROJECT_NAME} PRIVATE
  -fsanitize=address;
  -fsanitize=undefined)

add_custom_target(run_tests ALL
    DEPENDS ${PROJECT_NAME}
    COMMAND ${PROJECT_NAME}
    WORKING_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}>)

if(ENABLE_COVERAGE)
  target_compile_options(${PROJECT_NAME} PRIVATE -coverage)
  target_link_options(${PROJECT_NAME} PRIVATE -coverage)

  # find required tools
  find_program(LCOV lcov REQUIRED)
  find_program(GENHTML genhtml REQUIRED)

  # add coverage target
  add_custom_target(coverage ALL
    DEPENDS run_tests

    # gather data
    COMMAND ${LCOV} --directory . --capture --output-file coverage.info

    # generate report
    COMMAND ${GENHTML} --demangle-cpp -o coverage coverage.info

    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
endif()
