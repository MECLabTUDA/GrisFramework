
set(MODULE_NAME gstd)
add_subdirectory(${MODULE_NAME})
set_target_properties(${MODULE_NAME} PROPERTIES FOLDER Libraries)

if(BUILD_Testing)
  enable_testing()
  set(MODULE_NAME tests)
  add_subdirectory(${MODULE_NAME})
  
  # install all libraries for tests
  install(DIRECTORY "${GrisFramework_BUNDLE_DIR}/"
    DESTINATION             "tests"
    FILES_MATCHING PATTERN  "*${CMAKE_SHARED_LIBRARY_SUFFIX}")
  # install automatically depends on a previously executes bundling operation...
endif(BUILD_Testing)
#Use solution folders.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(_exports_file "GrisFrameworkTargets.cmake")
export(EXPORT GrisFramework FILE "cmake/${_exports_file}")
install(EXPORT GrisFramework DESTINATION "${GrisFramework_PACKAGE_INSTALL_DIR}" FILE "${_exports_file}")