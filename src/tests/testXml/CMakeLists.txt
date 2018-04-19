project(testXml)

set(INC_DIR ${CMAKE_SOURCE_DIR}/include/${MODULE_NAME}/${PROJECT_NAME}/)
	
set(Includes
	${INC_DIR}tests.h
	)
set(Private_Includes
	${INC_DIR}private/gris.pch
	)
set(Sources
	main.cpp
	tests.cpp
	)
	
source_group("Header Files\\private" FILES
	${Private_Includes}
	)
	
find_package(Boost 1.59.0 COMPONENTS filesystem unit_test_framework)
  
add_executable(${PROJECT_NAME}
	${Private_Includes}
	${Includes}
	${Sources}
	${Files}
	)

target_include_directories(${PROJECT_NAME} PUBLIC
	$<BUILD_INTERFACE:${INC_DIR}>
	$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
	)
	
target_link_libraries(${PROJECT_NAME}
	gstd
	pugixml
	Boost::filesystem
	Boost::unit_test_framework
	)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER ${MODULE_NAME})

# add version information to output file name
gris_append_product_version(${PROJECT_NAME} GrisFramework)
# ---------- post build
gris_deploy(${PROJECT_NAME} "${GrisFramework_BIN_DIR}" "${GrisFramework_Testing_DIR}")
# ---------- bundle and install
gris_bundle(${PROJECT_NAME})

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
  
