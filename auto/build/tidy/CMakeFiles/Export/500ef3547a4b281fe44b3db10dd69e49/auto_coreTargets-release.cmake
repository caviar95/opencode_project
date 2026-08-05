#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "auto::auto_core" for configuration "Release"
set_property(TARGET auto::auto_core APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(auto::auto_core PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libauto_core.a"
  )

list(APPEND _cmake_import_check_targets auto::auto_core )
list(APPEND _cmake_import_check_files_for_auto::auto_core "${_IMPORT_PREFIX}/lib/libauto_core.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
