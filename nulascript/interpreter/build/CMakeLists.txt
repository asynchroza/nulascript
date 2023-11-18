cmake_minimum_required(VERSION 3.12)

project(nulascript)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB NULASCRIPT_SUBDIRS "../../*")
foreach(subdir ${NULASCRIPT_SUBDIRS})
    if(IS_DIRECTORY ${subdir})
        include_directories(${subdir})
    endif()
endforeach()

file(GLOB SOURCE_FILES "../../*/*.cc")
list(FILTER SOURCE_FILES EXCLUDE REGEX ".*test.*\\.cc$")

add_executable(nulascript ${SOURCE_FILES} "main.cc")