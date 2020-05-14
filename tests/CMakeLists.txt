cmake_minimum_required(VERSION 3.12)

add_executable(rmq_tests 
  test_main.cpp
  unit/parsing_test.cpp
)

set_target_properties(rmq_tests PROPERTIES CXX_EXTENSIONS OFF)

find_package(doctest MODULE REQUIRED)

target_link_libraries(rmq_tests
  PRIVATE
    project_warnings
    project_options
    rmq
    doctest::doctest
)

add_test(NAME rmq_tests COMMAND rmq_tests)
