cmake_minimum_required(VERSION 2.8)
project(danjeros C ASM-ATT)

configure_file(${CMAKE_SOURCE_DIR}/config.h.in ${CMAKE_BINARY_DIR}/include/config.h)
include_directories(${CMAKE_BINARY_DIR}/include)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src)

