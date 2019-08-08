# ring_span requires C++14

if ("cxx_std_14" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
  add_executable(circex-p0059
    main.cpp
    )

  target_compile_features(circex-p0059 PUBLIC cxx_std_14)
  target_link_libraries(circex-p0059 trial-circular)
  add_dependencies(example circex-p0059)
else()
  message("Skipping p0059 example")
endif()
