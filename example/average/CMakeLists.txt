add_executable(circex-average
  main.cpp
  )

target_link_libraries(circex-average trial-circular)
add_dependencies(example circex-average)
