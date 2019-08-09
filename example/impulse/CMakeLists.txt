add_executable(circex-impulse
  main.cpp
  )

target_link_libraries(circex-impulse trial-circular)
add_dependencies(example circex-impulse)
