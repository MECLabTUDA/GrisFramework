macro(_gris_set_version_helper tag comp src)
  set(_${tag}_VERSION_${comp} ${${src}_VERSION_${comp}})
endmacro()

macro(_gris_derive_version_helper prod mod comp)
if(DEFINED ${prod}_VERSION_${comp})
  _gris_set_version_helper("PRODUCT" ${comp} ${prod})
  if(DEFINED ${prod}_${mod}_VERSION_${comp})
    _gris_set_version_helper("FILE" ${comp} ${prod}_${mod})
  else()
    _gris_set_version_helper("FILE" ${comp} ${prod})
  endif()
  set_property(GLOBAL PROPERTY ${mod}_VERSION_${comp} ${_FILE_VERSION_${comp}})
elseif()
  message(SEND_ERROR "The Version number of ${prod} is not properly defined: ${comp} missing.")
endif()
endmacro()

function(gris_configure_resource_rc GeneratedFile ProductName ProductSubName ModuleName)
# ProductName being the top name,
# ProductSubName being the Product Component name,
# ModuleName being the "main target name"

if(WIN32)
# prepare the Version information
  _gris_derive_version_helper(${ProductName} ${ModuleName} "MAJOR")
  _gris_derive_version_helper(${ProductName} ${ModuleName} "MINOR")
  _gris_derive_version_helper(${ProductName} ${ModuleName} "PATCH")
  _gris_derive_version_helper(${ProductName} ${ModuleName} "TWEAK")
  set_property(GLOBAL APPEND PROPERTY REGISTERED_MODULES "${ModuleName}")

# prepare the Project and Module information
  set("_FILE_NAME"        ${ModuleName})
  set("_PRODUCT_NAME"     ${ProductName})
  set("_PRODUCT_SUB_NAME" ${ProductSubName})
  set(_HEADERFILE)
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resource.h")
    set(_HEADERFILE "resource.h")
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/resource.h" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
  elseif(NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/resource.h")
    # What to do, if we have a resource.h in include directories?
    # --> use file(COPY ...) to copy the file manually
    message(STATUS [=[No resource.h in CURRENT_BINARY_DIR or CURRENT_SOURCE_DIR found, using gstd default file!
    To use a different resource.h-file, place it in CURRENT_SOURCE_DIR or file(COPY ...) it to CURRENT_BINARY_DIR.
    ]=])
    file(COPY "${GrisFramework_CMAKE_DIR}/resource.h" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
  endif()
  set(${GeneratedFile} "${CMAKE_CURRENT_BINARY_DIR}/resource.rc" ${_HEADERFILE} PARENT_SCOPE)
  
  configure_file("${GrisFramework_CMAKE_DIR}/resource.rc.in" "resource.rc")
  set_source_files_properties("${CMAKE_CURRENT_BINARY_DIR}/resource.rc" PROPERTIES)
endif()
endfunction()