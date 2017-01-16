project(testXml)

set(INC_DIR ${CMAKE_SOURCE_DIR}/include/${MODULE_NAME}/${PROJECT_NAME}/)

include_directories(
	${INC_DIR}
	${CMAKE_SOURCE_DIR}/include
	)

	
set(Includes
	)
set(Private_Includes
	${INC_DIR}private/testXml.pch
	)
set(Sources
	${PROJECT_NAME}.cpp
	)
	
source_group("Header Files\\private" FILES
	${Private_Includes}
	)
	
gris_boost_find_and_fix("1.59" COMPONENTS filesystem)
  
add_executable(${PROJECT_NAME}
	${Private_Includes}
	${Includes}
	${Sources}
	${Files}
	)
	
target_link_libraries(${PROJECT_NAME}
	gstd
	pugixml
	Boost::filesystem
  LOKI
	)

# add version information to output file name
gris_append_product_version(${PROJECT_NAME} GrisFramework)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER ${MODULE_NAME})

# ---------- post build

gris_deploy(${PROJECT_NAME} "${GrisFramework_BIN_DIR}" "${GrisFramework_Testing_DIR}")

# ---------- bundle and install

gris_bundle(${PROJECT_NAME})


add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
  
