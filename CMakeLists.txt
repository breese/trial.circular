###############################################################################
#
# Copyright (C) 2019 Bjorn Reese <breese@users.sourceforge.net>
#
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)
#
###############################################################################

cmake_minimum_required(VERSION 3.7)
project(trial.circular CXX)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(cmake)

if (MSVC)
  target_compile_options(trial-circular INTERFACE /W3)
else()
  target_compile_options(trial-circular INTERFACE -Wall -Wextra -pedantic -Werror=return-type)
endif()

add_subdirectory(example)
add_subdirectory(doc EXCLUDE_FROM_ALL)

enable_testing()
add_subdirectory(test)
