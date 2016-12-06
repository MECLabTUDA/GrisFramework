project(testLogger)

set(INC_DIR ${CMAKE_SOURCE_DIR}/include/${MODULE_NAME}/${PROJECT_NAME}/)

include_directories(
	$<BUILD_INTERFACE:${INC_DIR}>
	$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
	${LOKI_INCLUDE_DIR}	
	)

link_directories(
	${BOOST_LIB_DIR}
	${PUGI_LIB_DIR}
)

set(Sources
	${PROJECT_NAME}.cpp
	)	
set(Includes
	)
set(Private_Includes
	${INC_DIR}private/testLogger.pch
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
	
target_link_libraries(${PROJECT_NAME}
	gstd
	pugixml
	${BOOST_LIBS}
	)

# add version information to output file name
_gris_append_product_version(${PROJECT_NAME} GrisFramework)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER ${MODULE_NAME})
	
# ---------- post build

_gris_deploy_after_build(${PROJECT_NAME} ${CMAKE_BINARY_DIR}/$<CONFIG>/bin)

# ---------- bundle and install

_gris_bundle_target(${PROJECT_NAME})
_gris_bundle_add_lookup_directories(${BOOST_LIB_DIR})

install(TARGETS ${PROJECT_NAME} EXPORT GrisFramework
  RUNTIME DESTINATION ${GrisFramework_Testing_BIN_INSTALL_DIR}
  LIBRARY DESTINATION ${GrisFramework_Testing_LIB_INSTALL_DIR})
  