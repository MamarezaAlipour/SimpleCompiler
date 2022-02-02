cmake_minimum_required(VERSION 3.0)

project(x86_64_compiler)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}\
    -Wall -Wextra -Wno-long-long -pedantic -Wno-unused-result")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_EXTENSIONS off)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(X86_64_COMPILER ${PROJECT_NAME})
set(X86_64_TEST x86_64_test)

add_subdirectory(x86_64_compiler)
add_subdirectory(test)

set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)
