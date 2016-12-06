macro(_gris_deploy_after_build target dest)
# dest can include generator extressions

# make the folder
  add_custom_command(TARGET ${target} PRE_LINK COMMAND ${CMAKE_COMMAND} -E make_directory "${dest}")
# copy the files
  add_custom_command(TARGET ${target} POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:${target}>" "${dest}" 
    )
  if(MSVC)
  add_custom_command(TARGET ${target} POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      #"$<$<OR:$<CONFIG:RelWithDebInfo>,$<CONFIG:Debug>>:-E copy_if_different 
      "$<TARGET_FILE_DIR:${target}>/$<TARGET_PROPERTY:${target},OUTPUT_NAME>.pdb" "${dest}"#>"  
    )
  endif()
  get_property(_DEPLOY_NAME TARGET ${target} PROPERTY OUTPUT_NAME)
  get_property(_DEPLOY_SUFFIX TARGET ${target} PROPERTY SUFFIX)
  if(NOT _DEPLOY_NAME)
    get_property(_DEPLOY_NAME TARGET ${target} PROPERTY NAME)
  endif()
  
# TYPE STATIC_LIBRARY, MODULE_LIBRARY, SHARED_LIBRARY, INTERFACE_LIBRARY, EXECUTABLE
  set_property(TARGET ${target} PROPERTY DEPLOY_NAME "${dest}/${_DEPLOY_NAME}")
endmacro()