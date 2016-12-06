
set(MODULE_NAME gstd)
add_subdirectory(${MODULE_NAME})
set_target_properties(${MODULE_NAME} PROPERTIES FOLDER Libraries)

if(BUILD_Testing)
  set(MODULE_NAME tests)
  add_subdirectory(${MODULE_NAME})
endif(BUILD_Testing)
#Use solution folders.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(_exports_file "GrisFrameworkTargets.cmake")
export(EXPORT GrisFramework FILE "cmake/${_exports_file}")
install(EXPORT GrisFramework DESTINATION "lib/cmake/GrisFramework-${GrisFramework_SHORT_VERSION}" FILE "${_exports_file}")