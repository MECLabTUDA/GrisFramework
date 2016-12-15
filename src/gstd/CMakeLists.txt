project(gstd)

set(INC_DIR
	${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/)

set(Includes
	${INC_DIR}Vector.h
	${INC_DIR}XmlDocument.h
	${INC_DIR}XmlNode.h
	${INC_DIR}XmlAttribute.h
	${INC_DIR}DynamicProperty.h
	${INC_DIR}DynamicPropertyManager.h
	${INC_DIR}InvokeLater.h
	${INC_DIR}logger.h
	${INC_DIR}GrisFileLogger.h
	${INC_DIR}gstd_api.h
	)
set(Private_Includes
	${INC_DIR}private/gris.pch
	${INC_DIR}private/targetver.h
	${INC_DIR}resource.h
	)	
set(Sources	
	DynamicProperty.cpp
	DynamicPropertyManager.cpp
	logger.cpp
	GrisFileLogger.cpp
	XmlDocument.cpp
	XmlNode.cpp
	XmlAttribute.cpp
	Vector.cpp
	)

# configure the resource file
gris_configure_resource_rc(Resources GrisFramework std ${PROJECT_NAME})

gris_boost_find_and_fix("1.59" COMPONENTS filesystem)

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
	pugixml
  Boost::filesystem
  LOKI
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
gris_deploy_after_build(${PROJECT_NAME} "${GrisFramework_BUNDLE_DIR}")

# ---------- install

install(TARGETS gstd EXPORT GrisFramework
  LIBRARY ARCHIVE DESTINATION "${GrisFramework_LIB_INSTALL_DIR}"
  RUNTIME DESTINATION "${GrisFramework_BIN_INSTALL_DIR}"
  )
install(FILES ${Includes} DESTINATION "${GrisFramework_INCLUDE_INSTALL_DIR}/${PROJECT_NAME}" )
		 
	