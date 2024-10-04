cmake_minimum_required(VERSION 3.22)

project(SingleLinkedList)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "DEBUG")
endif()


add_compile_options(-Wall -std=gnu17)

add_library(libsinglelist STATIC single_list.c)

if(${CMAKE_BUILD_TYPE} STREQUAL "RELEASE")
    target_compile_options(libsinglelist PRIVATE -O3)
else()
    target_compile_options(libsinglelist PRIVATE -Wextra -Wpedantic -g)
endif()