project(gstd)

include_directories(
	${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/
	${BOOST_INCLUDE_DIR}
	${LOKI_INCLUDE_DIR}
	${PUGI_INCLUDE_DIR}
	)

link_directories(
	${BOOST_LIB_DIR}
	${PUGI_LIB_DIR}
)

set(INC_DIR
	${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/)

set(Includes
	${INC_DIR}XmlDocument.h
	${INC_DIR}XmlNode.h
	${INC_DIR}XmlAttribute.h
	${INC_DIR}DynamicProperty.h
	${INC_DIR}DynamicPropertyManager.h
	${INC_DIR}logger.h
	${INC_DIR}GrisFileLogger.h
	${INC_DIR}gstd_api.h
	)
set(Private_Includes
	${INC_DIR}private/gris.pch
	${INC_DIR}private/resource.h
	${INC_DIR}private/targetver.h	
	)
	
set(Sources
	dynamicPropertyManager.cpp
	logger.cpp
	GrisFileLogger.cpp
	XmlDocument.cpp
	XmlNode.cpp
	XmlAttribute.cpp
	)	
	
#set(Precompiled_Sources "")
	
#add_msvc_precompiled_header("muk_common.pch" "muk_common.cpp" ${Sources} )

add_library(gstd SHARED 
	${Sources}
	${Includes}
	${Private_Includes}
	)
	
target_link_libraries(
	gstd
	#xerces-c_3
	pugixml
	${BOOST_FILESYSTEM_R}
	)
	
source_group("Header Files\\private" FILES
	${Private_Includes}
	)

install(TARGETS gstd DESTINATION ${CMAKE_INSTALL_PREFIX}/gstd/
    )
	