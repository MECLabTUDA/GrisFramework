add_custom_target(BUNDLE ALL ${CMAKE_COMMAND} -P cmake_bundle.$<CONFIG>.cmake)
set_target_properties(BUNDLE
  PROPERTIES 
    FOLDER CMakePredefinedTargets
    APPLICATION_LIST ""
    LOOKUP_DIRECTORIES ""
    APPLICATION_FILE_DEFINITIONS ""
    DYNAMIC_LIB_DEFINITIONS "")

macro(_gris_bundle_target target)
  # prepare bundle target

  # bundle target dependency
  add_dependencies(BUNDLE ${target})

  # bundle addition
  set_property(TARGET BUNDLE APPEND PROPERTY APPLICATION_LIST ${target})

  set_property(TARGET BUNDLE APPEND_STRING PROPERTY APPLICATION_FILE_DEFINITIONS "\nset(${target}_file $<TARGET_FILE:${target}>)")

  if(${ARGC} GREATER 1)
    # additional dynamic Library dependencies
    set_property(TARGET BUNDLE APPEND_STRING PROPERTY DYNAMIC_LIB_DEFINITIONS "\nset(${target}_dynamic_libs ${ARGN})")
  endif()
endmacro()

macro(_gris_bundle_add_lookup_directories dirs)
  set_property(TARGET BUNDLE APPEND PROPERTY LOOKUP_DIRECTORIES ${dirs})
endmacro()

macro(_gris_configure_bundle_file)
  get_property(_BUNDLE_APPLICATION_LIST             TARGET BUNDLE PROPERTY APPLICATION_LIST)
  get_property(_BUNDLE_LOOKUP_DIRECTORIES           TARGET BUNDLE PROPERTY LOOKUP_DIRECTORIES)
  get_property(_BUNDLE_APPLICATION_FILE_DEFINITIONS TARGET BUNDLE PROPERTY APPLICATION_FILE_DEFINITIONS)
  get_property(_BUNDLE_DYNAMIC_LIB_DEFINITIONS      TARGET BUNDLE PROPERTY DYNAMIC_LIB_DEFINITIONS)

  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bundle)
  configure_file(cmake/bundle.cmake.in bundle/bundle.cmake @ONLY)

  file(GENERATE OUTPUT "${CMAKE_BINARY_DIR}/cmake_bundle.$<CONFIG>.cmake" INPUT "${CMAKE_BINARY_DIR}/bundle/bundle.cmake")
endmacro()

install(DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/bin"
  DESTINATION             "tests"
  FILES_MATCHING PATTERN  "*{CMAKE_SHARED_LIBRARY_SUFFIX}")
# install automatically depends on a previously executes bundling operation...
