# set initial values
# directory to deploy to
SET(GRIS_DEPLOY_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>" CACHE PATH "path to the deploy directory, may contain generator expressions")
SET(GRIS_LIB_INSTALL_DIRECTORY "lib" CACHE PATH "path to the library install directory")
MARK_AS_ADVANCED(GRIS_DEPLOY_DIRECTORY GrisFramework_LIB_DIR)

get_directory_property(hasParent PARENT_DIRECTORY)
if(hasParent)
  SET(GRIS_INSTALL_DEPLOYED True PARENT_SCOPE)
else()
  SET(GRIS_INSTALL_DEPLOYED True)
endif()

IF(NOT DEFINED GRIS_INSTALL_EXPORT)
  SET(GRIS_INSTALL_EXPORT "${PROJECT_NAME}_EXPORT" CACHE INTERNAL "Export Name of the install")
ENDIF()
    
add_custom_target(CLEAN_DEPLOY ${CMAKE_COMMAND} -E echo "cleaning $<CONFIG>-BUNDLE directories..." )

set_target_properties(CLEAN_DEPLOY
  PROPERTIES
    FOLDER CMakePredefinedTargets
    CLEAN_DIRECTORIES ""
    )

function(gris_deploy target sub)
# gris_deploy copies the target-file and its pdb-file into the specified 
# basepath/subfolder-folder. The folders can include generator extressions. folders will be created, if 
# they do not exist.
# If basepath is a relative path, it will be assumed to be relative to GRIS_DEPLOY_DIRECTORY. The subfolder 
# can be an empty string, in which case it is ignored. basepath will be stored in the target to provide a 
# common path, from which all of the targets deployment files will be copied to.
# 
# Paths are made absolute in this function based on the following rules:
# As long as the prefixed part of the path is relative more prefixes are added in the following order:
# 1. base, 2. ${GRIS_DEPLOY_DIRECTORY}, 3. ${CMAKE_BINARY_DIR} for deployment
# 1. base, 2. ${CMAKE_INSTALL_PREFIX} for install
# This also means, that subfolder MUST be relative
#
# ARGUMENTS
# gris_deploy(target subfolder [basepath])
  
  get_property(_DEPLOY_DIRECTORY TARGET ${target} PROPERTY DEPLOY_DIRECTORY)
  IF(_DEPLOY_DIRECTORY)
    MESSAGE(FATAL_ERROR "Trying to redeploy ${target}. Redeploying a Target to a secondary location is not supported.")
  ENDIF()
  GET_PROPERTY(lib_install_dir TARGET ${target} PROPERTY LIB_INSTALL_DIRECTORY)
  IF(NOT lib_install_dir)
    SET(lib_install_dir "${GRIS_LIB_INSTALL_DIRECTORY}")
  ENDIF()

  IF(${ARGC} EQUAL 3 AND NOT "${ARGN}" STREQUAL "")
    SET(base "${ARGN}/")
  ELSE()
    SET(base "")
  ENDIF()
  # remove prepended ${GRIS_DEPLOY_DIRECTORY)
  IF(NOT ${GRIS_DEPLOY_DIRECTORY} STREQUAL "")
    STRING(REPLACE "\\" "\\\\" _deploy_dir ${GRIS_DEPLOY_DIRECTORY})
    STRING(REPLACE "." "\\." _deploy_dir ${_deploy_dir})
    STRING(REGEX REPLACE "^${_deploy_dir}[\\\\/]*" "" base "${base}")
  ENDIF()
  _gris_ensure_slash_at_end("base" "${base}")
  _gris_ensure_slash_at_end("sub" "${sub}")
  _gris_ensure_slash_at_end("lib_install_dir" "${lib_install_dir}")
  
  # build the proper path
  IF((IS_ABSOLUTE "${sub}" OR IS_ABSOLUTE "${base}") AND GRIS_INSTALL_DEPLOYED)
    message(WARNING [=[using gris_deploy() with an absolute path. This may not lead to the desired install path.]=])
  ENDIF()
  _gris_prefix_folder(deploy_base "${base}" "${GRIS_DEPLOY_DIRECTORY}")
  # if still not absolute prefix CMAKE_BINARY_DIR
  _gris_prefix_folder(dest "${deploy_base}" "${CMAKE_BINARY_DIR}")

  _gris_prefix_folder(install_dir "${sub}" "${base}")
  _gris_prefix_folder(deploy_dir "${sub}" "${dest}")
  
  get_property(tgt_type TARGET ${target} PROPERTY TYPE)
  IF(tgt_type STREQUAL SHARED_LIBRARY OR tgt_type STREQUAL EXECUTABLE)
    _gris_copy_target_files(${target} ${target} "${deploy_dir}" "${install_dir}")
    
# clean the directory from deploy folder
    _gris_add_clean_directory("${deploy_dir}" "${install_dir}")
  ENDIF()
  
# TYPE STATIC_LIBRARY, MODULE_LIBRARY, SHARED_LIBRARY, INTERFACE_LIBRARY, EXECUTABLE
  set_property(TARGET ${target} PROPERTY DEPLOY_DIRECTORY "${deploy_base}")
  set_property(TARGET ${target} PROPERTY INSTALL_DIRECTORY "${base}")
  set_property(TARGET ${target} PROPERTY MAIN_DEPLOY_SUBDIRECTORY "${sub}")
  IF(GRIS_INSTALL_DEPLOYED)
    INSTALL(TARGETS ${target} EXPORT ${GRIS_INSTALL_EXPORT} 
      RUNTIME DESTINATION "${install_dir}" 
      ARCHIVE DESTINATION "${lib_install_dir}"
      LIBRARY DESTINATION "${lib_install_dir}")
    IF(MSVC AND tgt_type STREQUAL SHARED_LIBRARY)
      INSTALL(FILES "$<TARGET_PDB_FILE:${target}>" OPTIONAL DESTINATION "${install_dir}")
    ENDIF()
  ENDIF()
endfunction()

function(gris_deploy_files target sub)
# gris_deploy_files deploys additional files into the specified destination folder (dest). The deployment
# of these files is attached to the target passed. This function installs these files in the install 
# target, if GRIS_INSTALL_DEPLOYED is true. Call this multiple times for different sub-folders. The folder 
# will be created.
# subfolder has to be relative. The subfolder will be deleted on CLEAN target.
#
# ARGUMENTS
# gris_deploy_files(target subfolder file1 [file2 [...]])

  get_property(_DEPLOY_DIRECTORY  TARGET ${target} PROPERTY DEPLOY_DIRECTORY)
  get_property(_INSTALL_DIRECTORY TARGET ${target} PROPERTY INSTALL_DIRECTORY)
  if(NOT _DEPLOY_DIRECTORY)
    message(WARNING [=[using gris_deploy_files() without previously using gris_deploy() on the same 
    target may lead to unexpected results for the CLEAN_DEPLOY TARGET.]=])
  endif()
  
  if(${ARGC} GREATER 2)
    if(IS_ABSOLUTE ${sub})
      message(WARNING [=[using gris_deploy_files() with an absolute path. This will lead to unexpected results for the desired install path.]=])
    endif()
    # prefix deploy-dir (base and GRIS_DEPLOY_DIRECTORY at gris_deploy-time)
    _gris_prefix_folder(deploy_dir "${sub}" "${_DEPLOY_DIRECTORY}")
    # prefix CMAKE_BINARY_DIR
    _gris_prefix_folder(deploy_dir "${deploy_dir}" "${CMAKE_BINARY_DIR}")
    # prefix install_base to subfolder
    _gris_prefix_folder(install_dir "${sub}" "${_INSTALL_DIRECTORY}")
  # make the folder
    add_custom_command(TARGET ${target} PRE_LINK COMMAND ${CMAKE_COMMAND} -E make_directory "${deploy_dir}")
  # copy the files
    add_custom_command(TARGET ${target} POST_BUILD 
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${ARGN} "${deploy_dir}" 
      COMMENT "Copying Files ($<JOIN:${ARGN},$<COMMA> >) for ${target} to deploy directory ${install_dir}"
      )
    _gris_add_clean_directory("${deploy_dir}" "${install_dir}")
    set_property(TARGET ${target} PROPERTY HAS_DEPLOYED_FILES True)
    IF(GRIS_INSTALL_DEPLOYED)
      install(FILES ${ARGN} DESTINATION ${install_dir})
    ENDIF()
  else()
    message(WARNING "Calling gris_deploy_files() without any files to copy is undefined.")
  endif()
endfunction()

function(gris_deploy_directories target sub)
# gris_deploy_directories deploys files (and folders) from additional directories into the specified destination 
# folder (subfolder). The deployment of these directories is attached to the target passed. This function also 
# installs these directories in the install target. Call this multiple times for different sub-folders. The folder will 
# be created.
# Calling without any directories simply creates the destination directory. The destination path should be relative, 
# so the TARGETs DEPLOY_DIRECTORY will be prepended.
#
# EXAMPLE
# gris_deploy_directories(${target} "examples/deploy_dirs" "C:/examplefiles" "data/examples/deploy_dirs")
# This copies all files and folders in the folders "C:/examplefiles" "${CMAKE_SOURCE_DIR}/data/examples/deploy_dirs" into
# "$<deployfolder>/examples/deploy_dirs".
#
# ARGUMENTS
# gris_deploy_directories(target subfolder [directory1 [directory2 [...]]])

  get_property(_DEPLOY_DIRECTORY TARGET ${target} PROPERTY DEPLOY_DIRECTORY)
  get_property(_INSTALL_DIRECTORY TARGET ${target} PROPERTY INSTALL_DIRECTORY)
  if(NOT _DEPLOY_DIRECTORY)
    message(WARNING [=[using gris_deploy_directories() without previously using gris_deploy() on the same 
    target may lead to unexpected results for the CLEAN_DEPLOY TARGET.]=])
  endif()
  
  # prefix deploy-dir (base and GRIS_DEPLOY_DIRECTORY at gris_deploy-time)
  _gris_prefix_folder(deploy_dir "${sub}" "${_DEPLOY_DIRECTORY}")
  # prefix CMAKE_BINARY_DIR
  _gris_prefix_folder(deploy_dir "${deploy_dir}" "${CMAKE_BINARY_DIR}")
  # prefix install_base to subfolder
  _gris_prefix_folder(install_dir "${sub}" "${_INSTALL_DIRECTORY}")
  if(${ARGC} EQUAL 2)
    # just make the directory
    add_custom_command(TARGET ${target} POST_BUILD 
      COMMAND ${CMAKE_COMMAND} -E make_directory
        "${deploy_dir}" 
      COMMENT "Copying Directories ($<JOIN:${ARGN},$<COMMA> >) for ${target} to deploy directory ${install_dir}"
      )
    IF(GRIS_INSTALL_DEPLOYED)
      # create the folder in install target, make absolute path first
      SET(install_folder ${dest})
      IF(NOT IS_ABSOLUTE ${dest})
        SET(install_folder "${CMAKE_INSTALL_PREFIX}/${install_folder}")
      ENDIF(NOT IS_ABSOLUTE ${dest})
      INSTALL(CODE "FILE(MAKE_DIRECTORY \"${install_folder}\")")
    ENDIF()
  else()
    # copy the files
    add_custom_command(TARGET ${target} POST_BUILD 
      COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${ARGN} "${deploy_dir}" 
      COMMENT "Copying Directories ($<JOIN:${ARGN},$<COMMA> >) for ${target} to deploy directory ${install_dir}"
      )
    set_property(TARGET ${target} PROPERTY HAS_DEPLOYED_FILES True)
    IF(GRIS_INSTALL_DEPLOYED)
      INSTALL(DIRECTORY ${ARGN} DESTINATION ${install_dir})
    ENDIF()
  endif()
endfunction()

# -----------------
# PRIVATE FUNCTIONS
# -----------------

function(_gris_add_clean_directory folder rel_folder)
# clean directory
# make sure to not double-insert
  GET_PROPERTY(_CLEAN_DIRECTORIES TARGET CLEAN_DEPLOY PROPERTY CLEAN_DIRECTORIES)
  IF("${folder}" IN_LIST _CLEAN_DIRECTORIES)
# add clean folder command to clean target
    add_custom_command(TARGET CLEAN_DEPLOY POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E remove_directory
        "${folder}" 
      COMMENT "Removing deploy directory ${rel_folder}"
      )
    SET_PROPERTY(TARGET CLEAN_DEPLOY APPEND PROPERTY CLEAN_DIRECTORIES "${folder}")
  ENDIF()
endfunction()

function(_gris_prefix_folder out_var initial prefix)
  IF("${initial}" STREQUAL "")
    SET(out "${prefix}")
  ELSEIF("${prefix}" STREQUAL "")
    SET(out "${initial}")
  ELSEIF(IS_ABSOLUTE "${initial}")
    SET(out "${initial}")
  ELSE()
    _gris_ensure_slash_at_end(prefix "${prefix}")
    SET(out "${prefix}${initial}")
  ENDIF()
  SET(${out_var} ${out} PARENT_SCOPE)
endfunction()

function(_gris_ensure_slash_at_end var in)
  # make ${var} end on "/"
  IF(NOT ${in} MATCHES "[\\\\/]$" AND NOT "${in}" STREQUAL "")
    SET(${var} "${in}/" PARENT_SCOPE)
  ELSE()
    SET(${var} "${in}"  PARENT_SCOPE)
  ENDIF()
endfunction()

function(_gris_copy_target_files current_target target deploy_dir dir_name)
# make the folder
  add_custom_command(TARGET ${current_target} PRE_LINK 
    COMMAND ${CMAKE_COMMAND} -E make_directory "${deploy_dir}")
# copy the files
  #add_custom_command(TARGET ${current_target} POST_BUILD 
  #  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  #    "\"$<TARGET_FILE:${target}>\"" "$<$<OR:$<CONFIG:RelWithDebInfo>,$<CONFIG:Debug>>:\"$<TARGET_PDB_FILE:${target}>\">" "\"${deploy_dir}\""
  #  COMMENT "Copying Target ${target} to deploy directory ${dir_name}"
  #  )
  get_property(tgt_type TARGET "${current_target}" PROPERTY TYPE)
  if(WIN32 AND MSVC AND (${tgt_type} STREQUAL SHARED_LIBRARY OR ${tgt_type} STREQUAL EXECUTABLE))
# PDB_OUTPUT_DIRECTORY does not suppport generator expressions in cmake 3.8
# c.f. https://gitlab.kitware.com/cmake/cmake/issues/16365
# set_property(TARGET "${current_target}" PROPERTY PDB_OUTPUT_DIRECTORY "${deploy_dir}")
  add_custom_command(TARGET ${current_target} POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "$<$<OR:$<CONFIG:RelWithDebInfo>,$<CONFIG:Debug>>:\"$<TARGET_PDB_FILE:${target}>\">" "\"${deploy_dir}\""
    COMMENT "Copying Target ${target} to deploy directory ${dir_name}"
    )
  endif()
  set_property(TARGET "${current_target}" PROPERTY RUNTIME_OUTPUT_DIRECTORY "${deploy_dir}")
endfunction()

function(gris_set_lib_install target directory)
# set the library install folder of the target
  set_property(TARGET ${target} PROPERTY LIB_INSTALL_DIRECTORY "${directory}")
endfunction()