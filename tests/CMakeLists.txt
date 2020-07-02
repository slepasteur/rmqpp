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
)

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
