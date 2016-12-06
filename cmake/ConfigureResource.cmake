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
elseif()
  message(SEND_ERROR "The Version number of ${prod} is not properly defined: ${comp} missing.")
endif()
endmacro()

macro(_gris_configure_resource_rc ProductName ProductSubName ModuleName)
# ProductName being the top name,
# ProductSubName being the Product Component name,
# ModuleName being the "main target name"

if(WIN32)
# prepare the Version information
  _gris_derive_version_helper(${ProductName} ${ModuleName} "MAJOR")
  _gris_derive_version_helper(${ProductName} ${ModuleName} "MINOR")
  _gris_derive_version_helper(${ProductName} ${ModuleName} "PATCH")
  _gris_derive_version_helper(${ProductName} ${ModuleName} "TWEAK")

# prepare the Project and Module information
  set("_FILE_NAME"        ${ModuleName})
  set("_PRODUCT_NAME"     ${ProductName})
  set("_PRODUCT_SUB_NAME" ${ProductSubName})
  
  configure_file("${CMAKE_SOURCE_DIR}/cmake/resource.rc.in" "resource.rc")
endif()
endmacro()