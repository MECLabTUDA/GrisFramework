macro(gris_define_version ProjectName Major Minor Patch Tweak)
  set(${ProjectName}_VERSION_MAJOR    "${Major}")
  set(${ProjectName}_VERSION_MINOR    "${Minor}")
  set(${ProjectName}_VERSION_PATCH    "${Patch}")
  set(${ProjectName}_VERSION_TWEAK    "${Tweak}")
  set(${ProjectName}_VERSION          "${Major}.${Minor}.${Patch}.${Tweak}")
  set(${ProjectName}_SHORT_VERSION    "${Major}.${Minor}")
  
  IF(BUILD_SHARED)
    SET(${ProjectName}_LIBTYPE "SHARED")
  ELSE()
    SET(${ProjectName}_LIBTYPE "STATIC")
  ENDIF(BUILD_SHARED)
  
endmacro()

# SHARED LIB
OPTION(BUILD_SHARED "Want to build the included libs as shared?" ON)

