find_program(AsciiDoctor_COMMAND
  NAMES asciidoctor)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  AsciiDoctor
  REQUIRED_VARS AsciiDoctor_COMMAND
  )

mark_as_advanced(
  AsciiDoctor_FOUND
  AsciiDoctor_COMMAND)
