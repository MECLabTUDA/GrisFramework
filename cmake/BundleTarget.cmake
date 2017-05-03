# add_custom_target(BUNDLE ALL ${CMAKE_COMMAND} -D BUILD_TYPE=$<CONFIG> -P cmake_bundle.cmake)
add_custom_target(BUNDLE DEPENDS ALL_BUILD)

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
  
function(gris_bundle target)
# gris_bundle will add target to the BUNDLE target add try to add all dependent 
# libraries to the lookup_directories of the BUNDLE target. Additional dynamic libs, that are 
# dyamically loaded by the target may be added as addtional arguments. These will then bundled,
# even if their dependency is not uncovered for the executable by the dependency walker.
# Paths to dependening libraries will be added wherever possible, add additional paths with
# gris_bundle_add_lookup_directories(path) and gris_bundle_add_lookup_directories_config(config path).
# gris_bundle will also invoke the appropriate install commands.
# 
# ARGUMENTS
# gris_bundle(target [dynamic_lib1 [...]])

  # bundle target dependency
  add_dependencies(BUNDLE ${target})

  # bundle addition to target properties
  set_property(TARGET BUNDLE APPEND PROPERTY APPLICATION_LIST ${target})

  get_property(_MAIN_DEPLOY_SUBDIRECTORY  TARGET ${target} PROPERTY MAIN_DEPLOY_SUBDIRECTORY)
  get_property(_DEPLOY_DIRECTORY  TARGET ${target} PROPERTY DEPLOY_DIRECTORY)
  get_property(_INSTALL_DIRECTORY TARGET ${target} PROPERTY INSTALL_DIRECTORY)
  # prefix subfolder to the base folders
  _gris_prefix_folder(deploy_dir "${_MAIN_DEPLOY_SUBDIRECTORY}" "${_DEPLOY_DIRECTORY}")
  _gris_prefix_folder(deploy_dir "${deploy_dir}" "${CMAKE_BINARY_DIR}")
  _gris_prefix_folder(install_dir "${_MAIN_DEPLOY_SUBDIRECTORY}" "${_INSTALL_DIRECTORY}")
  if(_DEPLOY_DIRECTORY)
    # add file to apllication file definition
    set_property(TARGET BUNDLE APPEND_STRING PROPERTY APPLICATION_FILE_DEFINITIONS "\nset(${target}_file \"${deploy_dir}$<TARGET_FILE_NAME:${target}>\")")
    gris_bundle_add_lookup_directories(${deploy_dir})
    # clean the main deployment directory
    _gris_add_clean_directory("${deploy_dir}" "${install_dir}")
    
  else()
    set_property(TARGET BUNDLE APPEND_STRING PROPERTY APPLICATION_FILE_DEFINITIONS "\nset(${target}_file \"$<TARGET_FILE:${target}>\")")
    message(WARNING [=[using gris_bundle() without previously using gris_deploy() on the same target may lead to unexpected results for
    the BUNDLE TARGET, specifically, the BUNDLE will be created in the build folder instead of the deploy folder and deployment via
    gris_deploy will not work properly.]=])
  endif()

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
  IF(GRIS_INSTALL_DEPLOYED)
    # install all libraries for tests
    install(DIRECTORY "${deploy_dir}/"
      DESTINATION             "${install_dir}"
      FILES_MATCHING PATTERN  "*${CMAKE_SHARED_LIBRARY_SUFFIX}")
  ENDIF(GRIS_INSTALL_DEPLOYED)
endfunction()

function(gris_bundle_add_lookup_directories)
# Add a lookup directory to the lookip directories of the bundle target.
#
# ARGUMENTS
# gris_bundle_add_lookup_directories(path1 [path2 [...]])
  foreach(_dir IN LISTS ARGN)
    set_property(TARGET BUNDLE APPEND PROPERTY LOOKUP_DIRECTORIES "${_dir}")
  endforeach()
endfunction()

function(gris_bundle_add_lookup_directories_config _conf)
# Add a config-specific lookup directory to the lookip directories of the bundle target.
#
# ARGUMENTS
# gris_bundle_add_lookup_directories_config(config path1 [path2 [...]])
  string(TOUPPER ${_conf} _conf)
  foreach(_dir IN LISTS ARGN)
    set_property(TARGET BUNDLE APPEND PROPERTY LOOKUP_DIRECTORIES_${_conf} "${_dir}")
  endforeach()
endfunction()

function(gris_bundle_configure_file)
# gris_bundle_configure_file creates a bundle file, which is the target script to execute on 
# BUNDLE target execution. This should be the executed in the main CMakeLists.txt-file after all targets 
# have been added and all bundles are declared. 
# It creates a cmake_bundle.$<CONFIG>.cmake-file in the CMake Binary folder of the project for every configuration.
# gris_bundle_configure_file also creates the CLEAN_DEPLOY target.
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
  set(_prop LOCATION)
  if(${ARGC} GREATER 1)
    set(_prop "${_prop}_${ARGV1}")
  endif()
  get_property(_propval TARGET ${target} PROPERTY ${_prop})
  get_filename_component(_dir "${_propval}" DIRECTORY)
  if(${ARGC} GREATER 1)
    gris_bundle_add_lookup_directories_config(${_dir} ${ARGV1})
  else()
    gris_bundle_add_lookup_directories(${_dir})
  endif()
  
  gris_bundle_add_lookup_directories("$<TARGET_FILE_DIR:${target}>")
endfunction()

function(gris_bundle_add_library lib)
# gris_bundle_add_library adds libraries to the lookup directories of the bundle target. It uses the 
# information present in the targets PROPERTIES (i.e. DEPLOY_DIRECTORY from DeployAfter and 
# _gris_deploy) or the IMPORTED_LOCATION of IMPORTED targets.
# Internal targets (not IMPORTED) will use generator expressions. Non-targets will use the provided directory.
#
# ARGUMENTS
# gris_bundle_add_library(lib)

  if(NOT TARGET ${lib})
    get_filename_component(_ext ${lib} EXT)
    if("${_ext}" STREQUAL "${CMAKE_SHARED_LIBRARY_SUFFIX}")
      # else it would be a static library and not added
      get_filename_component(_dir "${lib}" DIRECTORY)
      message(WARNING "gris_bundle currently only works properly on TARGETs, but ${lib} is not defined as a TARGET.")
      IF(${_dir} STREQUAL "")
        message(WARNING "${lib} is only a file without a path, this is currently not properly supported by gris_bundle_add_library().")
      ELSE()
        gris_bundle_add_lookup_directories(${_dir})
      ENDIF()
    endif()
  else()
    get_property(_DEPLOY_DIRECTORY TARGET ${lib} PROPERTY DEPLOY_DIRECTORY)
    # if target is deployed via gris_deploy, use the 'deployed' version
    if(_DEPLOY_DIRECTORY)
      get_property(_MAIN_DEPLOY_SUBDIRECTORY TARGET ${lib} PROPERTY MAIN_DEPLOY_SUBDIRECTORY)
      IF(DEFINED target AND TARGET ${target})
        get_property(tgt_DEPLOY_DIRECTORY TARGET ${target} PROPERTY DEPLOY_DIRECTORY)
        get_property(tgt_MAIN_DEPLOY_SUBDIRECTORY TARGET ${target} PROPERTY MAIN_DEPLOY_SUBDIRECTORY)
        IF(tgt_DEPLOY_DIRECTORY AND NOT "${tgt_DEPLOY_DIRECTORY}${tgt_MAIN_DEPLOY_SUBDIRECTORY}" STREQUAL "${_DEPLOY_DIRECTORY}${_MAIN_DEPLOY_SUBDIRECTORY}")
          # add to the current target the command to copy libraries
          get_property(tgt_INSTALL_DIRECTORY TARGET ${target} PROPERTY INSTALL_DIRECTORY)
          _gris_prefix_folder(install_dir "${tgt_MAIN_DEPLOY_SUBDIRECTORY}" "${tgt_INSTALL_DIRECTORY}")
          _gris_prefix_folder(deploy_dir "${tgt_MAIN_DEPLOY_SUBDIRECTORY}" "${tgt_DEPLOY_DIRECTORY}")
          _gris_prefix_folder(deploy_dir "${deploy_dir}" "${CMAKE_BINARY_DIR}")
  
          _gris_copy_target_files(${target} ${lib} "${deploy_dir}" "${install_dir}")
        # ELSE: deployed to the correct folder anyways
        ENDIF()
      ELSE()
        # instead of bundling through the build location, deploy to path
        _gris_prefix_folder(dir "${_MAIN_DEPLOY_SUBDIRECTORY}" "${_DEPLOY_DIRECTORY}")
        _gris_prefix_folder(dir "${dir}" "${CMAKE_BINARY_DIR}")
        # use DEPLOY_DIRECTORY property
        gris_bundle_add_lookup_directories("${dir}")
      ENDIF()
    else()
      get_property(_IMPORTED TARGET ${lib} PROPERTY IMPORTED)
      if(_IMPORTED AND ${_IMPORTED})
      
        gris_bundle_add_lookup_directories_from_imported_target(${lib})
        foreach(_conf ${CMAKE_CONFIGURATION_TYPES})
          gris_bundle_add_lookup_directories_from_imported_target(${lib} ${_conf})
        endforeach()
      elseif()
        gris_bundle_add_lookup_directories("$<TARGET_FILE_DIR:${lib}>")
      endif()
    endif()
  endif()
endfunction()
