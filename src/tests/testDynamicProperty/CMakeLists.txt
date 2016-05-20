project(testDynamicProperty)

set(INC_DIR ${CMAKE_SOURCE_DIR}/include/${MODULE_NAME}/${PROJECT_NAME}/)

include_directories(
	${INC_DIR}
	${CMAKE_SOURCE_DIR}/include
	${LOKI_INCLUDE_DIR}	
	${BOOST_INCLUDE_DIR}
	)

link_directories(
	${PUGI_LIB_DIR}
	${BOOST_LIB_DIR}
)

set(Sources	
	tests.cpp
	TestClass.cpp
	boostTestCase.cpp
	)	
set(Includes
	${INC_DIR}tests.h
	${INC_DIR}TestClass.h
	)
set(Private_Includes
	${INC_DIR}private/test.pch
	)
set(Files
	${PROJECT_NAME}_deploy.bat
	)

add_executable(${PROJECT_NAME}
	${Private_Includes}
	${Includes}
	${Sources}
	${Files}
	)

source_group("Header Files\\private" FILES
	${Private_Includes}
	)
	
source_group("Post-Build Files" FILES
	${PROJECT_NAME}_deploy.bat
	)
	
target_link_libraries(${PROJECT_NAME}
	gstd
	${BOOST_LIBS}
	)
	
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER ${MODULE_NAME})

ADD_CUSTOM_COMMAND(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}_deploy.bat
		${PROJECT_NAME}
		${MODULE_NAME}
		${CMAKE_BINARY_DIR}
		${CMAKE_INSTALL_PREFIX}\\${MODULE_NAME}\\${PROJECT_NAME}
	)

SET(APPS "${CMAKE_INSTALL_PREFIX}/${MODULE_NAME}/${PROJECT_NAME}/${PROJECT_NAME}.exe")

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/tests/${PROJECT_NAME}/
    BUNDLE DESTINATION /tests/${PROJECT_NAME} COMPONENT Runtime
    RUNTIME DESTINATION /tests/${PROJECT_NAME} COMPONENT Runtime
    )
	
SET(DIRS 
	${BOOST_LIB_DIR}
	${CMAKE_INSTALL_PREFIX}/gstd/
	)	
	
INSTALL(CODE "
    file(WRITE \"\${CMAKE_INSTALL_PREFIX}/${MODULE_NAME}/${PROJECT_NAME}/bundle.conf\")
    " COMPONENT Runtime)

	
INSTALL(CODE "
	file(GLOB_RECURSE VAR_NAME
      \"\${CMAKE_INSTALL_PREFIX}/${MODULE_NAME}/${PROJECT_NAME}/*${CMAKE_SHARED_LIBRARY_SUFFIX}\")
    include(BundleUtilities)
    fixup_bundle(\"${APPS}\" \"\${VAR_NAME}\" \"${DIRS}\")
    " COMPONENT Runtime)