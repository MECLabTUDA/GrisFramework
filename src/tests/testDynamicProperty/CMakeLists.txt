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
	
add_executable(${PROJECT_NAME}
	${Private_Includes}
	${Includes}
	${Sources}
	)
		
target_include_directories(${PROJECT_NAME} PUBLIC
	$<BUILD_INTERFACE:${INC_DIR}>
	$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
	${LOKI_INCLUDE_DIR}	
	${BOOST_INCLUDE_DIR}
	)
	
target_link_libraries(${PROJECT_NAME}
	gstd
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