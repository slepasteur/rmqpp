cmake_minimum_required(VERSION 3.12)

add_executable(rmq_tests 
  test_main.cpp
  unit/parsing_data_field_test.cpp
)

find_package(doctest MODULE REQUIRED)

target_link_libraries(rmq_tests
  PRIVATE
    rmq::rmq
    parse_it::parse_it
    doctest::doctest
)

set_target_properties(rmq_tests PROPERTIES CXX_EXTENSIONS OFF)

add_test(NAME rmq_tests COMMAND rmq_tests)
