###############################################################################
#
# Copyright (C) 2019 Bjorn Reese <breese@users.sourceforge.net>
#
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)
#
###############################################################################

cmake_minimum_required(VERSION 3.0)
project(trial.circular)

get_filename_component(TRIAL_CIRCULAR_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../" ABSOLUTE)
if (NOT DEFINED TRIAL_CIRCULAR_BUILD_DIR)
  set(TRIAL_CIRCULAR_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})
endif()
set(LIBRARY_OUTPUT_PATH ${TRIAL_CIRCULAR_BUILD_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${TRIAL_CIRCULAR_BUILD_DIR}/bin)

# Require at least C++11
if (${CMAKE_VERSION} VERSION_LESS 3.8)
  set(TRIAL_CIRCULAR_FEATURES cxx_auto_type)
else()
  set(TRIAL_CIRCULAR_FEATURES cxx_std_11)
endif()

###############################################################################
# Trial.Circular
###############################################################################

add_library(trial-circular INTERFACE)

set_target_properties(trial-circular PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${TRIAL_CIRCULAR_ROOT}/include"
  INTERFACE_COMPILE_FEATURES "${TRIAL_CIRCULAR_FEATURES}")

install(DIRECTORY ${TRIAL_CIRCULAR_ROOT}/include
  DESTINATION .
  FILES_MATCHING PATTERN "*.[hi]pp"
  )
