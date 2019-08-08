find_package(Threads)

add_executable(circex-concurrent
  main.cpp
  )

target_link_libraries(circex-concurrent trial-circular Threads::Threads)
