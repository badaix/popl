# This file is part of popl (program options parser lib)
# Copyright (C) 2015-2017 Johannes Pohl

# This software may be modified and distributed under the terms
# of the MIT license.  See the LICENSE file for details.

cmake_minimum_required(VERSION 2.4) 

project(PoplTest)

include_directories(
	"include"
)

add_executable(PoplTest
	PoplTest.cpp
)


SET(CMAKE_INSTALL_INCLUDEDIR include CACHE PATH "Output directory for header files")
		
install(FILES include/popl.hpp DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

