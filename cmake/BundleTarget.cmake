# add_custom_target(BUNDLE ALL ${CMAKE_COMMAND} -D BUILD_TYPE=$<CONFIG> -P cmake_bundle.cmake)
add_custom_target(BUNDLE ALL)

set_target_properties(BUNDLE
  PROPERTIES 
    FOLDER CMakePredefinedTargets
    APPLICATION_LIST ""
    LOOKUP_DIRECTORIES ""
    APPLICATION_FILE_DEFINITIONS ""
    DYNAMIC_LIB_DEFINITIONS "")
foreach(_conf ${CMAKE_CONFIGURATION_TYPES})
  string(TOUPPER ${_conf} _upper_conf)
  set_target_properties(BUNDLE PROPERTIES LOOKUP_DIRECTORIES_${_upper_conf} "")
endforeach()

function(gris_bundle_target target)
# gris_bundle_target will add target to the BUNDLE target add try to add all dependent 
# libraries to the lookup_directories of the BUNDLE target. Additional dynamic libs, that are 
# dyamically loaded by the target may be added as addtional arguments. These will then bundled,
# even if their dependency is not uncovered for the executable by the dependency walker.
# Paths to dependening libraries will be added wherever possible, add additional paths with
# gris_bundle_add_lookup_directories(path) and gris_bundle_add_lookup_directories_config(config path)
# 
# ARGUMENTS
# gris_bundle_target(target [dynamic_lib1 [...]])

  # bundle target dependency
  add_dependencies(BUNDLE ${target})

  # bundle addition to target properties
  set_property(TARGET BUNDLE APPEND PROPERTY APPLICATION_LIST ${target})
  set_property(TARGET BUNDLE APPEND_STRING PROPERTY APPLICATION_FILE_DEFINITIONS "\nset(${target}_file $<TARGET_FILE:${target}>)")

  if(${ARGC} GREATER 1)
    # additional dynamic Library dependencies
    set_property(TARGET BUNDLE APPEND_STRING PROPERTY DYNAMIC_LIB_DEFINITIONS "\nset(${target}_dynamic_libs ${ARGN})")
    math(EXPR _end "${ARGC}-1")
    foreach(_i RANGE 1 ${ARGC} ${_end})
      gris_bundle_add_library(${ARGV${_i}})
    endforeach()
  endif()
  
  # auto-add lookup_directories of shared dependent libraries
  # get_property(_)
  # get shared libraries and add their BINARY_DIR
  gris_get_dependent_libraries(_dep_targets ${target} TYPE SHARED_LIBRARY)
  foreach(_lib IN LISTS _dep_targets)
    gris_bundle_add_library(${_lib})
  endforeach()
  
endfunction()

function(gris_bundle_add_lookup_directories dirs)
# Add a lookup directory to the lookip directories of the bundle target.
#
# ARGUMENTS
# gris_bundle_add_lookup_directories(path)
  set_property(TARGET BUNDLE APPEND PROPERTY LOOKUP_DIRECTORIES "${dirs}")
endfunction()

function(gris_bundle_add_lookup_directories_config _conf dirs)
# Add a config-specific lookup directory to the lookip directories of the bundle target.
#
# ARGUMENTS
# gris_bundle_add_lookup_directories_config(config path)
  string(TOUPPER ${_conf} _conf)
  set_property(TARGET BUNDLE APPEND PROPERTY LOOKUP_DIRECTORIES_${_conf} "${dirs}")
endfunction()

function(gris_bundle_configure_file)
# gris_bundle_configure_file creates a bundle file, which is the target script to execute on 
# BUNDLE target execution. This should be the executed in the main CMakeLists.txt-file after all targets 
# have been added and all bundles are declared. 
# It creates a cmake_bundle.$<CONFIG>.cmake-file in the CMake Binary folder of the project for every configuration.
#
# ARGUMENTS
# gris_bundle_configure_file()
  get_property(_BUNDLE_LOOKUP_DIRECTORIES           TARGET BUNDLE PROPERTY LOOKUP_DIRECTORIES)
  if(_BUNDLE_LOOKUP_DIRECTORIES)
    list(REMOVE_DUPLICATES _BUNDLE_LOOKUP_DIRECTORIES)
  endif()
  get_property(_BUNDLE_APPLICATION_FILE_DEFINITIONS TARGET BUNDLE PROPERTY APPLICATION_FILE_DEFINITIONS)
  get_property(_BUNDLE_DYNAMIC_LIB_DEFINITIONS      TARGET BUNDLE PROPERTY DYNAMIC_LIB_DEFINITIONS)

  configure_file(${GrisFramework_CMAKE_DIR}/bundle.cmake.in cmake/bundle.cmake.in @ONLY)
  file(GENERATE OUTPUT "${CMAKE_BINARY_DIR}/cmake_bundle.$<CONFIG>.cmake" INPUT "${CMAKE_BINARY_DIR}/cmake/bundle.cmake.in")
  add_custom_command(TARGET BUNDLE POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -P "cmake_bundle.$<CONFIG>.cmake" 
    COMMENT "Bundling all targets..."
    VERBATIM)
endfunction()

function(gris_bundle_add_lookup_directories_from_imported_target target)
# gris_bundle_add_lookup_directories_from_imported_target adds bundle lookup directories 
# depending on the imported target locations of an imported target
# the function only uses the property IMPORTED_LOCATION or, if config is set, the corresponding 
# IMPORTED_LOCATION_<config>
#
# ARGUMENTS
# gris_bundle_add_lookup_directories_from_imported_target(target [config])
  set(_prop IMPORTED_LOCATION)
  if(${ARGC} GREATER 1)
    set(_prop "${_prop}_${ARGV1}")
  endif()
  get_property(_propval TARGET ${target} PROPERTY ${_prop})
  get_filename_component(_dir "${_propval}" DIRECTORY)
  gris_bundle_add_lookup_directories(${_dir})
endfunction()

function(gris_bundle_add_library lib)
# gris_bundle_add_library adds libraries to the lookup directories of the bundle target. It uses the 
# information present in the targets PROPERTIES (i.e. DEPLOY_NAME from DeployAfter and 
# _gris_deploy_after_build) or the IMPORTED_LOCATION of IMPORTED targets.
# Internal targets (not IMPORTED) will use generator expressions. Non-targets will use the provided directory.
#
# ARGUMENTS
# gris_bundle_add_library(lib)
  if(NOT TARGET ${lib})
    get_filename_component(_ext ${lib} EXT)
    if("${_ext}" STREQUAL "${CMAKE_SHARED_LIBRARY_SUFFIX}")
      # else it would be a static library and not added
      get_filename_component(_dir "${lib}" DIRECTORY)
      gris_bundle_add_lookup_directories(${_dir})
      
      message(WARNING [=[
      gris_bundle currently only works properly on TARGETs, but ${lib} is not defined as a TARGET.
      ]=])
    endif()
  else()
    get_property(_DEPLOY_NAME TARGET ${lib} PROPERTY DEPLOY_NAME)
    if(_DEPLOY_NAME)
      # use DEPOLY_NAME property
      get_filename_component(_DEPLOY_NAME "${_DEPLOY_NAME}" DIRECTORY)
      gris_bundle_add_lookup_directories("${_DEPLOY_NAME}")
    else()
      get_property(_IMPORTED TARGET ${lib} PROPERTY IMPORTED)
      if(_IMPORTED AND ${_IMPORTED})
      
        gris_bundle_add_lookup_directories_from_imported_target(${lib})
        foreach(_conf ${CMAKE_CONFIGURATION_TYPES})
          gris_bundle_add_lookup_directories_from_imported_target(${lib} ${_conf})
        endforeach()
      elseif()
        gris_bundle_add_lookup_directories("$<TARGET_FILE_DIR:${lib}>")
      
        message(WARNING [=[
        gris_bundle currently uses deploy locations or imported locations
        ]=])
      endif()
    endif()
  endif()

# function(gris_bundle_add_boost)
	# foreach(_conf ${CMAKE_CONFIGURATION_TYPES})
	  # string(TOUPPER ${_conf} _upper_conf)
	  # if(${_conf} STREQUAL "Debug")
      # gris_bundle_add_lookup_directories_config(${_conf} "${Boost_LIBRARY_DIR_${_upper_conf}}")
	  # else()
      # gris_bundle_add_lookup_directories_config(${_conf} "${Boost_LIBRARY_DIR_RELEASE}")
	  # endif()
# endforeach()
endfunction()
