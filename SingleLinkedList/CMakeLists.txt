cmake_minimum_required(VERSION 3.22)

project(SingleLinkedList)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "DEBUG")
endif()


add_compile_options(-Wall -std=gnu17)

add_library(singlelist STATIC src/single_list.c)

if(${CMAKE_BUILD_TYPE} STREQUAL "RELEASE")
    target_compile_options(singlelist PRIVATE -O3)
else()
    target_compile_options(singlelist PRIVATE -Wextra -Wpedantic -g)
endif()

add_executable(test src/test.c)
target_compile_options(test PRIVATE -g)
if (${CMAKE_BUILD_TYPE} STREQUAL "RELEASE")
    target_link_directories(test PRIVATE ${CMAKE_SOURCE_DIR}/lib)

    set_target_properties(
        test
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    )
    set_target_properties(
        singlelist
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib
    )
endif()
target_link_libraries(test PRIVATE singlelist)

