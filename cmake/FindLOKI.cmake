# setting the LOKI properties for CMake modules
#
# FindLOKI will export an imported library interface target to link to
#
# LOKI_DIR is the path to the LOKI include path
#
# Exported Target LOKI


# define LOKI_DIR as an "CACHE VARIABLE" and initialize

if(NOT LOKI_DIR)
  if(DEFINED SDK_BASEPATH)
    set(_SDK_BASEPATH "${SDK_BASEPATH}")
  else()
    set(_SDK_BASEPATH "${CMAKE_INSTALL_PREFIX}")
  endif()
  set(LOKI_DIR "${_SDK_BASEPATH}/loki-0.1.7" CACHE PATH "Specifies the loki include directory")
endif()

if(NOT TARGET LOKI AND LOKI_DIR)
  # check, whether all files are there
  set(_LOKI_INC_DIR "${LOKI_DIR}/loki/")
  set(_LOKI_IN_DIR ON)
  set(LOKI_FOUND ON)
  foreach(_file AbstractFactory AssocVector EmptyType Factory Functor HierarchyGenerators MultiMethods NullType Singleton SmallObj SmartPtr static_check Threads Tuple TypeInfo Typelist TypeManip TypeTraits Visitor)
    if(NOT EXISTS "${_LOKI_INC_DIR}${_file}.h")
      set(_LOKI_IN_DIR OFF)
      set(LOKI_FOUND   OFF)
      message(STATUS "${_file} missing for LOKI in LOKI_DIR: ${LOK_DIR}")
    endif()
  endforeach()

  add_library(LOKI INTERFACE IMPORTED)

  set_target_properties(LOKI PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${LOKI_DIR}"
  )
  if(NOT ${_LOKI_IN_DIR})
    message("LOKI could not be found")
  endif()
endif()