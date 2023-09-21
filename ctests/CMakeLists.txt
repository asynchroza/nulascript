cmake_minimum_required(VERSION 3.12)

project(tests)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB NULASCRIPT_SUBDIRS "../nulascript/*")
foreach(subdir ${NULASCRIPT_SUBDIRS})
    if(IS_DIRECTORY ${subdir})
        include_directories(${subdir})
    endif()
endforeach()

add_subdirectory("../googletest" [EXCLUDE_FROM_ALL])

file(GLOB SOURCE_FILES "../nulascript/*/*.cc")

add_executable(tests ${SOURCE_FILES} "../nulascript/tests/lexer_test.cc")

target_link_libraries(tests gtest gtest_main pthread)
