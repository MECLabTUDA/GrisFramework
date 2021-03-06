macro(gris_debug_print_vars)
  get_cmake_property(_variableNames VARIABLES)
  foreach (_variableName ${_variableNames})
    if(${ARGC} GREATER 0)
      if("${_variableName}" MATCHES ${ARGV0})
        message(STATUS "${_variableName}=${${_variableName}}")
      endif()
    else()
      message(STATUS "${_variableName}=${${_variableName}}")
    endif()
  endforeach()  
endmacro()

# Get all propreties that cmake supports
execute_process(COMMAND ${CMAKE_COMMAND} --help-property-list OUTPUT_VARIABLE CMAKE_TARGET_PROPERTY_LIST)

# Convert command output into a CMake list
STRING(REGEX REPLACE ";" "\\\\;" CMAKE_TARGET_PROPERTY_LIST "${CMAKE_TARGET_PROPERTY_LIST}")
STRING(REGEX REPLACE "\n" ";" CMAKE_TARGET_PROPERTY_LIST "${CMAKE_TARGET_PROPERTY_LIST}")

set(CMAKE_TARGET_PROPERTY_WHITELIST "INTERFACE_INCLUDE_DIRECTORIES" "NAME" "TYPE" "ALIASED_TARGET" "IMPORTED")

function(gris_debug_print_target_property _target _prop)
  # message ("Checking ${_prop}")
  set(_CMP0026_TYPES "LOCATION" "VS_DEPLOYMENT_LOCATION" "MACOSX_PACKAGE_LOCATION")
  foreach(_conf ${CMAKE_CONFIGURATION_TYPES})
    set(_CMP0026_TYPES ${_CMP0026_TYPES} "LOCATION_${_conf}")
  endforeach()
  if("${_prop}" IN_LIST _CMP0026_TYPES)
    set(_CMP0026 ON)
  else()
    set(_CMP0026 OFF)
  endif()
  if(${_CMP0026})  
    cmake_policy(PUSH)
    cmake_policy(SET CMP0026 OLD) # allow LOCATION target properties
  endif()
  get_property(_prop_value TARGET ${_target} PROPERTY ${_prop} SET)
  if (_prop_value)
    get_target_property(_prop_value ${_target} ${_prop})
    if(${_CMP0026})  
      message("Beware of using LOCATION properties, it might contain post-generator-time replacements (CMP0026).")
    endif()
    message (STATUS "[Target ${_target}] ${_prop} = ${_prop_value}")
  endif()
  if(${_CMP0026})
    cmake_policy(POP)
  endif()
endfunction(gris_debug_print_target_property)

function(gris_debug_print_properties)
    message ("CMAKE_TARGET_PROPERTY_LIST = ${CMAKE_TARGET_PROPERTY_LIST}")
endfunction(gris_debug_print_properties)

function(gris_debug_print_target _target)
# gris_debug_print_target is a helper function to help debug targets. It will automatically 
# print all properties of a target that are returned by `cmake --help-property-list`. Additionally
# custom properties also passed to gris_debug_print_target will be retrieved and printed.
#
# ARGUMENTS
# gris_debug_print_target(target [additional_properties [...]])
  if(NOT TARGET ${_target})
    message("There is no target named '${_target}'")
    return()
  endif()

  get_target_property(_target_type ${_target} TYPE)
  set(_interface OFF)
  if(${_target_type} MATCHES "^INTERFACE_")
    set(_interface ON)
  endif()
  set(_property_list ${CMAKE_TARGET_PROPERTY_LIST} ${ARGN})
  list(SORT _property_list)
  foreach (_prop ${_property_list})
    if(NOT ${_interface} OR ${_prop} IN_LIST CMAKE_TARGET_PROPERTY_WHITELIST)
      if(${_prop} MATCHES "<CONFIG>")
        foreach(_config IN LISTS CMAKE_CONFIGURATION_TYPES)
          string(REPLACE "<CONFIG>" "${_config}" _prop_i ${_prop})
          gris_debug_print_target_property(${_target} ${_prop_i})
        endforeach(_config)
      else()
        gris_debug_print_target_property(${_target} ${_prop})
      endif()
    endif()
  endforeach(_prop)
endfunction(gris_debug_print_target)  
