cmake_minimum_required(VERSION 2.8)
project(danjeros C ASM-ATT)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type for make-style systems" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

configure_file(${CMAKE_SOURCE_DIR}/config.h.in ${CMAKE_BINARY_DIR}/include/config.h)
include_directories(${CMAKE_BINARY_DIR}/include)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src)

find_program(CTAGS ctags)
if(CTAGS)
    get_target_property(CTAGS_SOURCES ${CMAKE_PROJECT_NAME} SOURCES)
    add_custom_target(ctags
        COMMAND ${CTAGS} -o ${CMAKE_BINARY_DIR}/tags -R ${CMAKE_SOURCE_DIR}/include --tag-relative=yes ${CTAGS_SOURCES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src VERBATIM)
endif(CTAGS)

find_program(CSCOPE cscope)
if(CSCOPE)
    get_target_property(CSCOPE_SOURCES ${CMAKE_PROJECT_NAME} SOURCES)
    add_custom_target(cscope
        COMMAND ${CSCOPE} -b -f ${CMAKE_BINARY_DIR}/cscope.out -I ${CMAKE_SOURCE_DIR}/include/ ${CSCOPE_SOURCES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src VERBATIM)
endif(CSCOPE)

