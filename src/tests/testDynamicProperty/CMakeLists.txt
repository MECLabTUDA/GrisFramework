project(testDynamicProperty)

set(INC_DIR ${CMAKE_SOURCE_DIR}/include/${MODULE_NAME}/${PROJECT_NAME}/)
	
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

add_executable(${PROJECT_NAME}
	${Private_Includes}
	${Includes}
	${Sources}
	)
		
target_include_directories(${PROJECT_NAME} PUBLIC
	$<BUILD_INTERFACE:${INC_DIR}>
	$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
	)
	
target_link_libraries(${PROJECT_NAME}
	gstd
	Boost::dynamic_linking
	Boost::filesystem
	Boost::unit_test_framework
	)

# add version information to output file name
gris_append_product_version(${PROJECT_NAME} GrisFramework)
	
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER ${MODULE_NAME})

# ---------- post build

gris_deploy(${PROJECT_NAME} "${GrisFramework_BIN_DIR}" "${GrisFramework_Testing_DIR}")

# ---------- bundle and install

gris_bundle(${PROJECT_NAME})