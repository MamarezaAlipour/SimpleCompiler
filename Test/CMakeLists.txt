file(GLOB_RECURSE X86_64_TEST_SRC ${CMAKE_SOURCE_DIR} *.cpp *.hpp)

add_executable(${X86_64_TEST}
    ${X86_64_TEST_SRC})

target_link_libraries(${X86_64_TEST}
    PRIVATE ${X86_64_COMPILER})
