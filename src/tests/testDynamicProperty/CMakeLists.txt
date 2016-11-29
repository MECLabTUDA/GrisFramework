project(testDynamicProperty)

set(INC_DIR ${CMAKE_SOURCE_DIR}/include/${MODULE_NAME}/${PROJECT_NAME}/)

link_directories(
	${PUGI_LIB_DIR}
	${BOOST_LIB_DIR}
)
	
set(Private_Includes
	${INC_DIR}private/test.pch
	)
set(Includes
	${INC_DIR}custom_streams.h
	${INC_DIR}TestClass.h
	${INC_DIR}testDynamicPropertyManager.h
	${INC_DIR}tests.h
	)
set(Sources	
	main.cpp
	TestClass.cpp
	testDynamicPropertyManager.cpp	
	tests.cpp
	)
	
source_group("Header Files\\private" FILES
	${Private_Includes}
	)
	
source_group("Post-Build Files" FILES
	${PROJECT_NAME}_deploy.bat
	)

add_executable(${PROJECT_NAME}
	${Private_Includes}
	${Includes}
	${Sources}
	)
		
target_include_directories(${PROJECT_NAME} PUBLIC
	${INC_DIR}
	${CMAKE_SOURCE_DIR}/include
	${LOKI_INCLUDE_DIR}	
	${BOOST_INCLUDE_DIR}
	)
	
target_link_libraries(${PROJECT_NAME}
	gstd
	${BOOST_LIBS}
	)
	
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER ${MODULE_NAME})

# ---------- post build and install

set(IPATH ${CMAKE_BINARY_DIR}/src/)
set(OPATH ${CMAKE_INSTALL_PREFIX}/apps/)

file(MAKE_DIRECTORY ${OPATH})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${IPATH}${MODULE_NAME}/${PROJECT_NAME}/$<CONFIG>/${PROJECT_NAME}.exe		${OPATH})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${IPATH}${MODULE_NAME}/${PROJECT_NAME}/$<CONFIG>/${PROJECT_NAME}.pdb		${OPATH})
	
set(Application "${OPATH}/${PROJECT_NAME}.exe")

install(CODE "
    file(WRITE \"${OPATH}/bundle_${PROJECT_NAME}.conf\")
    " COMPONENT Runtime)
	
set(Bin_Lookup_Directories 
	${GRIS_BIN_DIR}	
	${BOOST_LIB_DIR}	
	)
	
install(CODE "
	file(GLOB_RECURSE VAR_NAME
      \"${OPATH}/*${CMAKE_SHARED_LIBRARY_SUFFIX}\")
    include(BundleUtilities)
    fixup_bundle(\"${Application}\" \"${VAR_NAME}\" \"${Bin_Lookup_Directories}\")
    " COMPONENT Runtime)