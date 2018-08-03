project(gstd)

set(INC_DIR
  ${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/)

set(Includes
  ${INC_DIR}ContainerStreamOperators.h
  ${INC_DIR}DynamicPropertyException.h
  ${INC_DIR}BaseVector.h
  ${INC_DIR}Exception.h
  ${INC_DIR}VectorImpl.h
  ${INC_DIR}Vector.h
  ${INC_DIR}XmlDocument.h
  ${INC_DIR}XmlNode.h
  ${INC_DIR}XmlAttribute.h
  ${INC_DIR}XmlDynamicPropertyParser.h
  ${INC_DIR}DynamicProperty.h
  ${INC_DIR}DynamicPropertyManager.h
  ${INC_DIR}IDynamicPropertyParser.h
  ${INC_DIR}IObservable.h
  ${INC_DIR}IObservableWithReturn.h
  ${INC_DIR}InvokeLater.h
  ${INC_DIR}logger.h
  ${INC_DIR}GrisFileLogger.h
  ${INC_DIR}gstd_api.h
  )
set(Private_Includes
  ${INC_DIR}private/targetver.h
  )	
set(Sources	
  DynamicProperty.cpp
  DynamicPropertyException.cpp
  Exception.cpp
  IDynamicPropertyParser.cpp
  DynamicPropertyManager.cpp
  logger.cpp
  GrisFileLogger.cpp
  XmlDocument.cpp
  XmlNode.cpp
  XmlAttribute.cpp
  XmlDynamicPropertyParser.cpp
  Vector.cpp
  )

SET(Private_LIBRARIES )

if(${GrisFramework_USE_JSON})
  SET(Includes
    ${Includes}
    ${INC_DIR}JSONDynamicPropertyParser.h
    )
  SET(Sources
    ${Sources}
    JSONDynamicPropertyParser.cpp
    )
endif()


# configure the resource file
gris_configure_resource_rc(Resources GrisFramework std ${PROJECT_NAME})

add_library(${PROJECT_NAME} ${GrisFramework_LIBTYPE}
  ${Includes}
  ${Private_Includes}
  ${Sources}
  ${Resources}
  )

# public includes
target_include_directories( ${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/>
  $<INSTALL_INTERFACE:${GrisFramework_INCLUDE_INSTALL_DIR}>
  ${Boost_INCLUDE_DIRS}
  )

if(${GrisFramework_USE_JSON})
  target_include_directories(${PROJECT_NAME} PRIVATE
    ${RapidJSON_INCLUDE_DIR}
    )
  TARGET_COMPILE_DEFINITIONS(${PROJECT_NAME} PRIVATE
    RAPIDJSON_HAS_STDSTRING
  )
endif()

#private includes
target_link_libraries( ${PROJECT_NAME}	PRIVATE
  pugixml
  LOKI
  ${Private_LIBRARIES}
  )

target_link_libraries( ${PROJECT_NAME} PUBLIC 
	Boost::dynamic_linking
	Boost::filesystem
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
     
  