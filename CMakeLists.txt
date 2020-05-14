cmake_minimum_required(VERSION 3.12)

project(rmq
  VERSION 0.1
  DESCRIPTION "A RabbitMQ client library."
  LANGUAGES CXX)

add_library(rmq INTERFACE)

target_compile_features(rmq INTERFACE cxx_std_20)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    add_subdirectory(tests)
endif()

