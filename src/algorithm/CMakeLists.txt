project(algorithm)

set(INC_DIR
	${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/)

set(Includes
	${INC_DIR}clamp.h
  ${INC_DIR}dummy.h
	${INC_DIR}algorithm_api.h
	)
set(Private_Includes
	${INC_DIR}private/targetver.h
	)	
set(Sources	
  dummy.cpp
	)

# configure the resource file
gris_configure_resource_rc(Resources GrisFramework std ${PROJECT_NAME})

add_library(${PROJECT_NAME} SHARED
	${Includes}
	${Private_Includes}
	${Sources}
	${Resources}
	)
# public includes
target_include_directories( ${PROJECT_NAME} PUBLIC
	$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/>
  $<INSTALL_INTERFACE:${GrisFramework_INCLUDE_INSTALL_DIR}>
  )
  
#private includes
target_link_libraries( ${PROJECT_NAME}	PRIVATE
  )
	
source_group("Header Files\\private" FILES
	${Private_Includes}
	)
source_group("Resources" FILES
	${Resources}
	)

# add version information to output file name
gris_append_product_version(${PROJECT_NAME} GrisFramework)

# ---------- post build 
# is is using ${PROJECT_NAME}=gstd as target
gris_deploy(${PROJECT_NAME} "${GrisFramework_BIN_DIR}" "")

# ---------- install includes
install(FILES ${Includes} DESTINATION "${GrisFramework_INCLUDE_INSTALL_DIR}/${PROJECT_NAME}" )
		 
	