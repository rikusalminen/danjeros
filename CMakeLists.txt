cmake_minimum_required(VERSION 2.8)
project(danjeros C ASM-ATT)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type for make-style systems" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

configure_file(${CMAKE_SOURCE_DIR}/config.h.in ${CMAKE_BINARY_DIR}/include/config.h)
include_directories(${CMAKE_BINARY_DIR}/include)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src)

