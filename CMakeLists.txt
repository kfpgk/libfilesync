cmake_minimum_required(VERSION 3.20)

project(libfilesync VERSION 0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(BUILD_UNIT_TESTS "Build unit tests" OFF)
option(BUILD_INTEGRATION_TESTS "Build integration tests" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)

if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
    add_compile_definitions(DEBUG_BUILD=1)
endif()

add_subdirectory(libfilesync)

if(BUILD_UNIT_TESTS OR BUILD_INTEGRATION_TESTS)
    enable_testing()
endif()

if(BUILD_INTEGRATION_TESTS)
    add_subdirectory(tests)
endif()

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()