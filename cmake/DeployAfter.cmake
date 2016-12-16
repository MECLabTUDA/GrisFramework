function(gris_deploy_after_build target dest)
# gris_deploy_after_build copies the target-file and its pdb-file into the specified 
# dest-folder. The folder dest can include generator extressions. dest will be created, if it 
# does not exist.
# 
# ARGUMENTS
# gris_deploy_after_build(target dest)

# make the folder
  add_custom_command(TARGET ${target} PRE_LINK COMMAND ${CMAKE_COMMAND} -E make_directory "${dest}")
# copy the files
  add_custom_command(TARGET ${target} POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "$<TARGET_FILE:${target}>" "$<TARGET_PDB_FILE:${target}>" "${dest}" 
    COMMENT "Copying Target ${target} to deploy directory ${dest}"
    )
  get_property(_DEPLOY_NAME TARGET ${target} PROPERTY OUTPUT_NAME)
  get_property(_DEPLOY_SUFFIX TARGET ${target} PROPERTY SUFFIX)
  if(NOT _DEPLOY_NAME)
    get_property(_DEPLOY_NAME TARGET ${target} PROPERTY NAME)
  endif()
  
# TYPE STATIC_LIBRARY, MODULE_LIBRARY, SHARED_LIBRARY, INTERFACE_LIBRARY, EXECUTABLE
  set_property(TARGET ${target} PROPERTY DEPLOY_NAME "${dest}/${_DEPLOY_NAME}")
endfunction()

function(gris_deploy_files target dest)
# gris_deploy_files deploys additional files into the specified destination folder (dest). The deployment
# of these files is attached to the target passed. This function does not install these files in the install 
# target. Call this multiple times for different sub-folder. The folder will be created.
#
# ARGUMENTS
# gris_deploy_files(target dest file1 [file2 [...]])

  if(${ARGC} GREATER 2)

  # make the folder
    add_custom_command(TARGET ${target} PRE_LINK COMMAND ${CMAKE_COMMAND} -E make_directory "${dest}")
  # copy the files
    add_custom_command(TARGET ${target} POST_BUILD 
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${ARGN} "${dest}" 
      COMMENT "Copying Files ($<JOIN:${ARGN},$<COMMA> >) for ${target} to deploy directory ${dest}"
      )
  else()
    message(WARNING "Calling gris_deploy_files() without any files to copy...")
  endif()
endfunction()