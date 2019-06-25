find_package(AsciiDoctor QUIET)

function(trial_circular_add_doc name source)
  add_custom_target(${name} ALL
    COMMAND ${AsciiDoctor_COMMAND} -b html -D html ${CMAKE_CURRENT_SOURCE_DIR}/${source}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
endfunction()

trial_circular_add_doc(trial-circular-doc-span span.adoc)

if (AsciiDoctor_FOUND)

  add_custom_target(doc
    DEPENDS trial-circular-doc-span
    )

else() # No AsciiDoctor
  add_custom_target(doc
    ${CMAKE_COMMAND} -E cmake_echo_color --red "Please install AsciiDoctor to build documentation."
    )

endif()
