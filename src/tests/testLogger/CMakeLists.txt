project(testLogger)

set(INC_DIR ${CMAKE_SOURCE_DIR}/include/${MODULE_NAME}/${PROJECT_NAME}/)

include_directories(
	$<BUILD_INTERFACE:${INC_DIR}>
	$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
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
  LOKI
	)

# add version information to output file name
gris_append_product_version(${PROJECT_NAME} GrisFramework)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER ${MODULE_NAME})
	
# ---------- post build

gris_deploy_after_build(${PROJECT_NAME} "${GrisFramework_BUNDLE_DIR}")

# ---------- bundle and install

gris_bundle_target(${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME} EXPORT GrisFramework
  RUNTIME DESTINATION ${GrisFramework_Testing_BIN_INSTALL_DIR}
  LIBRARY DESTINATION ${GrisFramework_Testing_LIB_INSTALL_DIR})
  