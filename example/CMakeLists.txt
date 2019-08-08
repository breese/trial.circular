add_subdirectory(average)

add_custom_target(example DEPENDS
  circex-average
  )
