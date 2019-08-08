add_subdirectory(average)
add_subdirectory(concurrent)

add_custom_target(example DEPENDS
  circex-average
  circex-concurrent
  )
