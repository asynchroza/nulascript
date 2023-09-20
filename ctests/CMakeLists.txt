cmake_minimum_required(VERSION 3.12)

project(tests)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory("../googletest" [EXCLUDE_FROM_ALL])

file(GLOB SOURCE_FILES "../nulascript/*")

add_executable(tests ${SOURCE_FILES} ../nulascript/tests/example_unittest.cc)

target_link_libraries(tests gtest gtest_main pthread)
