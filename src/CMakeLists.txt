
set(MODULE_NAME gstd)
add_subdirectory(${MODULE_NAME})
set_target_properties(${MODULE_NAME} PROPERTIES FOLDER Libraries)

set(MODULE_NAME tests)
add_subdirectory(${MODULE_NAME})

#Use solution folders.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)


