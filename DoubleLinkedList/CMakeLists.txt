cmake_minimum_required(VERSION 3.22)

project(DoubleLinkedList)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "DEBUG")
endif()

if(${CMAKE_BUILD_TYPE} STREQUAL "RELEASE")
    set(COMPILE_OPTIONS -O3)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "SANITIZE")
    set(COMPILE_OPTIONS -g -Wextra -Wpedantic -fsanitize=address)
    set(LINK_OPTIONS -fsanitize=address)
else()
    set(COMPILE_OPTIONS -g -Wextra -Wpedantic)
endif()


add_compile_options(-Wall -std=gnu17 ${COMPILE_OPTIONS})
add_link_options(${LINK_OPTIONS})

add_library(doublelist STATIC src/double_list.c)
add_executable(test src/test.c)
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
target_link_libraries(test PRIVATE doublelist)

