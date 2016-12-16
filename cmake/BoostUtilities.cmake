# Map RelWithDebInfo and MinSizRel to Release, if specific targets are not available
# this is especially required for Boost (since FindBoost will otherwise map those to Debug)
# See also: https://cmake.org/Bug/bug_relationship_graph.php?bug_id=16091&graph=relation
#           https://gitlab.kitware.com/cmake/cmake/issues/16091
# This issue might be fixed in a more current version of CMake

function(_gris_boost_fix_map_imported_config)
# _gris_boost_fix_map_imported_config() walks through all passed targets and their dependencies 
# and sets MAP_IMPORTED_CONFIG_MINSIZEREL and MAP_IMPORTED_CONFIG_RELWITHDEBINFO to Release
# so Debug will not be choosen. This is a fix for an issue with pre cmake-3.7 behaviour.
#
# ARGUMENTS
# _gris_boost_fix_map_imported_config(target1 [target2 [...]])
  set(_recursive_targets ${ARGN})
  foreach(_tgt ${ARGN})
    if(NOT TARGET ${_tgt})
      message(SEND_ERROR "Target ${_tgt} does not exist")
    endif()
#    message("fixed ${_tgt}")
    gris_get_dependent_libraries(_link_libraries ${_tgt})
    if(_link_libraries)
      set(_recursive_targets ${_recursive_targets} ${_link_libraries})
    endif()
  endforeach()
  if(_recursive_targets)
    list(REMOVE_DUPLICATES _recursive_targets)
    foreach(_tgt ${_recursive_targets})
      set_target_properties(${_tgt} PROPERTIES MAP_IMPORTED_CONFIG_MINSIZEREL "Release")
      set_target_properties(${_tgt} PROPERTIES MAP_IMPORTED_CONFIG_RELWITHDEBINFO "Release")
    endforeach()
  endif()
endfunction()

function(gris_boost_find_and_fix)
# gris_boost_find_and_fix() calls find_package("Boost" ${ARGN}) and follows that up with a call to 
# fix the mapped import configs (that is using minsizerel and relwithdebinfo). Imported Boost Library
# Targets are also provided in the Variable BOOST_LIBRARIES. (internal Boost dependencies are not resolved 
# for BOOST_LIBRARIES, but they are implicitly handled by CMAKE)
#
# ARGUMENTS
# gris_boost_find_and_fix([vars]) --> vars are passed through to find_package, use find_package syntax

  find_package("Boost" ${ARGN})
  set(_complist "Boost::boost")
  set(_args ${ARGN})
  if("COMPONENTS" IN_LIST _args)
    set(_arglist "EXACT" "QUIET" "REQUIRED" "CONFIG" "NO_MODULE" "NO_POLICY_SCOPE" 
      "NAMES" "CONFIGS" "HINTS" "PATHS" "PATH_SUFFIXES" "NO_DEFAULT_PATH" "NO_CMAKE_ENVIRONMENT_PATH"
      "NO_CMAKE_PATH" "NO_SYSTEM_ENVIRONMENT_PATH" "NO_CMAKE_PACKAGE_REGISTRY" "NO_CMAKE_BUILDS_PATH" 
      "NO_CMAKE_SYSTEM_PATH" "NO_CMAKE_SYSTEM_PACKAGE_REGISTRY" "CMAKE_FIND_ROOT_PATH_BOTH" 
      "ONLY_CMAKE_FIND_ROOT_PATH" "NO_CMAKE_FIND_ROOT_PATH")
    list(FIND _args "COMPONENTS" _c_pos)
    math(EXPR _c_pos "${_c_pos}+1")
    math(EXPR _end "${ARGC}-1")
    foreach(_loop RANGE ${_c_pos} ${_end} 1)
      list(GET _args ${_loop} _c_arg)
      if("${_c_arg}" IN_LIST _arglist)
        break()
      endif()
      list(APPEND _complist "Boost::${_c_arg}")
    endforeach()
  endif()
  _gris_boost_fix_map_imported_config(${_complist})
  set(_complist ${BOOST_LIBRARIES} ${_complist})
  list(REMOVE_DUPLICATES _complist)
  set(BOOST_LIBRARIES ${_complist} PARENT_SCOPE)
endfunction()
