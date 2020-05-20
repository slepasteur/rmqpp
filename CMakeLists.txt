cmake_minimum_required(VERSION 3.12)

project(rmq
  VERSION 0.1
  DESCRIPTION "A RabbitMQ client library."
  LANGUAGES CXX)

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)
set_target_properties(project_options PROPERTIES CXX_EXTENSIONS OFF)

# Sanitizer options if supported by compiler.
# Enable by using -DSANITIZE_ADDRESS=ON / -DSANITIZE_MEMORY=ON
#  / -DSANITIZE_UNDEFINED_BEHAVIOR=ON / -DSANITIZE_THREAD=ON
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)


add_library(rmq INTERFACE)
target_include_directories(rmq
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(rmq
  INTERFACE
    project_warnings
    project_options
)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    add_subdirectory(tests)
endif()

