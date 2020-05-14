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

# Sanitizer options if supported by compiler.
# Enable by using -DENABLE_SANITIZER_ADDRESS=ON / -DENABLE_SANITIZER_MEMORY=ON
#  / -DENABLE_SANITIZER_UNDEFINED_BEHAVIOR=ON / -DENABLE_SANITIZER_THREAD=ON
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)


add_library(rmq INTERFACE)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    add_subdirectory(tests)
endif()

