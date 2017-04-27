
set(MODULE_NAME gstd)
add_subdirectory(${MODULE_NAME})
set_target_properties(${MODULE_NAME} PROPERTIES FOLDER Libraries)
set(MODULE_NAME algorithm)
add_subdirectory(${MODULE_NAME})
set_target_properties(${MODULE_NAME} PROPERTIES FOLDER Libraries)

if(BUILD_Testing)
  enable_testing()
  set(MODULE_NAME tests)
  add_subdirectory(${MODULE_NAME})
  
  # install automatically depends on a previously executes bundling operation...
endif(BUILD_Testing)
#Use solution folders.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(_exports_file "GrisFrameworkTargets.cmake")
export(EXPORT ${GRIS_INSTALL_EXPORT} FILE "cmake/${_exports_file}")
install(EXPORT ${GRIS_INSTALL_EXPORT} DESTINATION "${GrisFramework_PACKAGE_INSTALL_DIR}" FILE "${_exports_file}")