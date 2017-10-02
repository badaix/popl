#  ____   __  ____  __   
# (  _ \ /  \(  _ \(  )  
#  ) __/(  O )) __// (_/\
# (__)   \__/(__)  \____/

# This file is part of popl (program options parser lib)
# Copyright (C) 2015-2017 Johannes Pohl

# This software may be modified and distributed under the terms
# of the MIT license.  See the LICENSE file for details.

cmake_minimum_required(VERSION 2.4) 

project(popl_example)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(
	"include"
)

add_executable(popl_example
	popl_example.cpp
)


SET(CMAKE_INSTALL_INCLUDEDIR include CACHE PATH "Output directory for header files")
		
install(FILES include/popl.hpp DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

