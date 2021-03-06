cmake_minimum_required(VERSION 3.12)

project(rmq
  VERSION 0.1
  DESCRIPTION "A RabbitMQ client library."
  LANGUAGES CXX)

# Export compile commands and copy them inside the root directory for YCM
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")
IF( EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
  EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
    ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
  )
ENDIF()

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)

# Sanitizer options if supported by compiler.
# Enable by using -DSANITIZE_ADDRESS=ON / -DSANITIZE_MEMORY=ON
#  / -DSANITIZE_UNDEFINED_BEHAVIOR=ON / -DSANITIZE_THREAD=ON
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)


add_library(rmq 
  src/transport/parsing/byte_span_to_string.cpp
  src/transport/serialization/serialize.cpp
  src/transport/parsing/parse.cpp
  src/transport/connection.cpp
  src/types.cpp
  src/utils/print.cpp
)
add_library(rmq::rmq ALIAS rmq)

target_include_directories(rmq
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

find_package(fmt MODULE REQUIRED)

target_link_libraries(rmq
  PUBLIC
    parse_it::parse_it
  PRIVATE
    project_options
    project_warnings
    fmt::fmt
)
set_target_properties(rmq PROPERTIES CXX_EXTENSIONS OFF)

add_subdirectory(extern/parse_it)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)
endif()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
  add_subdirectory(tests)
endif()

