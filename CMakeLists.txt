#  ____   __  ____  __   
# (  _ \ /  \(  _ \(  )  
#  ) __/(  O )) __// (_/\
# (__)   \__/(__)  \____/

# This file is part of popl (program options parser lib)
# Copyright (C) 2015-2021 Johannes Pohl

# This software may be modified and distributed under the terms
# of the MIT license.  See the LICENSE file for details.

cmake_minimum_required(VERSION 2.4) 

if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
    endif ()
else ()
    set (CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(PROJECT_VERSION "1.3.0")
endif ()


project(popl_example)

set(PROJECT_DESCRIPTION "Header-only C++ program options parser library")
set(PROJECT_URL "https://github.com/badaix/popl")

option(BUILD_EXAMPLE "Build example (build popl_example demo)" ON)
option(BUILD_TESTS "Build tests" ON)


if(NOT DEFINED CMAKE_INSTALL_INCLUDEDIR)
    SET(CMAKE_INSTALL_INCLUDEDIR include CACHE
        PATH "Output directory for header files")
endif()


include_directories(
	"include"
)

install(FILES include/popl.hpp DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

if (BUILD_EXAMPLE)
add_subdirectory(example)
endif (BUILD_EXAMPLE)

if (BUILD_TESTS)
add_subdirectory(test)
endif (BUILD_TESTS)

