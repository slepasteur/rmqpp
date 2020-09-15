cmake_minimum_required(VERSION 3.12)

add_executable(rmq_tests 
  test_main.cpp
  unit/parsing/parsing_data_field_test.cpp
  unit/parsing/parsing_field_value_test.cpp
  unit/parsing/parsing_field_table_test.cpp
  unit/parsing/parsing_messages_test.cpp
  unit/serializing/serializing_messages_test.cpp
  unit/serializing/serializing_data_fields_test.cpp
  unit/connection_test.cpp
  unit/utils/split_long_string_test.cpp
  unit/utils/byte_range_to_short_string_test.cpp
)

target_include_directories(rmq_tests
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR})

find_package(doctest MODULE REQUIRED)
find_package(Boost REQUIRED)

target_link_libraries(rmq_tests
  PRIVATE
    project_options
    project_warnings
    rmq::rmq
    doctest::doctest
    fmt::fmt
)

set_target_properties(rmq_tests PROPERTIES CXX_EXTENSIONS OFF)

add_test(NAME rmq_tests COMMAND rmq_tests)

add_executable(connect_test
  manual/connect.cpp
)

target_link_libraries(connect_test
  PRIVATE
    project_options
    project_warnings
    rmq::rmq
    Boost::Boost
    fmt::fmt
)
