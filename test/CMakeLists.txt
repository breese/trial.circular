###############################################################################
#
# Copyright (C) 2019 Bjorn Reese <breese@users.sourceforge.net>
#
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)
#
###############################################################################

# Boost.Core/Detail (lightweight_test)
find_package(Boost 1.57.0 QUIET)

function(trial_circular_add_test name)
  add_executable(${name} ${ARGN})
  target_include_directories(${name} SYSTEM PRIVATE "${Boost_INCLUDE_DIRS}")
  target_link_libraries(${name} trial-circular)
  add_test(${name} ${EXECUTABLE_OUTPUT_PATH}/${name})
  target_compile_options(${name} PRIVATE ${TRIAL_CIRCULAR_WARNING_FLAGS})
  target_compile_features(${name} PRIVATE ${TRIAL_CIRCULAR_FEATURES})
endfunction()

trial_circular_add_test(span_suite span_suite.cpp)
trial_circular_add_test(span_algorithm_suite span_algorithm_suite.cpp)
trial_circular_add_test(span_iterator_suite span_iterator_suite.cpp)
trial_circular_add_test(span_numeric_suite span_numeric_suite.cpp)

trial_circular_add_test(array_suite array_suite.cpp)
trial_circular_add_test(array_numeric_suite array_numeric_suite.cpp)
