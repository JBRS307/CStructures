cmake_minimum_required(VERSION 3.22)

project(ThredPool)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "DEBUG")
endif()

if(${CMAKE_BUILD_TYPE} STREQUAL "RELEASE")
    set(COMPILE_OPTIONS -03)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "SANITIZE")
    set(COMPILE_OPTIONS -g -Wall -Wextra -fsanitize=address)
    set(LINK_OPTIONS -fsanitize=address)
else()
    set(COMPILE_OPTIONS -g -Wall -Wextra)
endif()

add_library(threadpool STATIC src/thread_pool.c)

add_executable(test src/test.c)
add_dependencies(test threadpool)

if(${CMAKE_BUILD_TYPE} STREQUAL "RELEASE")
    target_link_directories(test PRIVATE ${CMAKE_SOURCE_DIR/lib})    

    set_target_properties(
        test
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    )

    set_target_properties(
        threadpool
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib
    )
endif()
target_link_options(test PRIVATE threadpool)