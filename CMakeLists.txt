cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(GrisFramework)

# generate Version variables
include(cmake/version.cmake)

# set INSTALL dirs 
set(GrisFramework_BIN_INSTALL_DIR "bin")
set(GrisFramework_LIB_INSTALL_DIR "lib")
set(GrisFramework_INCLUDE_INSTALL_DIR "include/GrisFramework-${GrisFramework_SHORT_VERSION}")
set(GrisFramework_PACKAGE_INSTALL_DIR "lib/cmake/GrisFramework-${GrisFramework_SHORT_VERSION}")
set(GrisFramework_Testing_BIN_INSTALL_DIR "tests")
set(GrisFramework_Testing_LIB_INSTALL_DIR "tests/lib")

set(GrisFramework_CMAKE_DIR "${CMAKE_SOURCE_DIR}/cmake/")
set(GrisFramework_BUNDLE_DIR "${CMAKE_BINARY_DIR}/$<CONFIG>/bin")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${GrisFramework_CMAKE_DIR})

# define _gris_deploy_after_build
include(cmake/DeployAfter.cmake)
# define _gris_configure_resource_rc
include(cmake/ConfigureResource.cmake)
# define _gris_bundle_target
include(cmake/BundleTarget.cmake)
# define _gris_*version
include(cmake/TargetAppendVersion.cmake)
# define _gris_fix_map_imported_config (and _gris_bundle_boost)
include(cmake/BoostUtilities.cmake)
include(cmake/DependencyWalker.cmake)
include(cmake/PugiUtilities.cmake)

include(cmake/DebugCMake.cmake)

set(SDK_BASEPATH CACHE PATH "specifies the sdk base path")

if (SDK_BASEPATH STREQUAL "")
  message(SEND_ERROR "Set SDK_BASEPATH to the directory containing the 3rd party libraries")
endif()

option(BUILD_Testing "enable tests" OFF)



set(Boost_NO_BOOST_CMAKE ON)
set(BOOST_ROOT "${SDK_BASEPATH}/boost_1_59_0" CACHE PATH "Boost root directory")
set(BOOST_LIBRARYDIR "${BOOST_ROOT}/lib32-msvc-14.0"  CACHE PATH "Boost library directory")
mark_as_advanced(BOOST_LIBRARYDIR)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_DEBUG_RUNTIME OFF)
# set(Boost_DEBUG ON)

gris_boost_find_and_fix("1.59")

find_package(LOKI)

gris_pugixml_find_and_fix()

add_subdirectory(src)

file(GLOB GrisFramework_MACROS_LIST RELATIVE "${GrisFramework_CMAKE_DIR}" "cmake/*.cmake")
# cmake files not to install/include
list(REMOVE_ITEM GrisFramework_MACROS_LIST Version.cmake)
set(GrisFramework_MACROS_LIST_FULL )
foreach(_file IN LISTS GrisFramework_MACROS_LIST)
  set(GrisFramework_MACROS_LIST_FULL ${GrisFramework_MACROS_LIST_FULL} "cmake/${_file}")
endforeach()

# install *.in files
file(GLOB AdditionalFiles               RELATIVE "${CMAKE_SOURCE_DIR}" "cmake/*.in")
list(REMOVE_ITEM AdditionalFiles cmake/GrisFrameworkConfig.cmake.in)
list(REMOVE_ITEM AdditionalFiles cmake/GrisFrameworkConfigVersion.cmake.in)
set(AdditionalFiles ${AdditionalFiles} "cmake/resource.h")

configure_file(cmake/GrisFrameworkConfig.cmake.in         GrisFrameworkConfig.cmake @ONLY)
configure_file(cmake/GrisFrameworkConfigVersion.cmake.in  GrisFrameworkConfigVersion.cmake @ONLY)

# install macros
install(FILES ${GrisFramework_MACROS_LIST_FULL} ${AdditionalFiles} DESTINATION "${GrisFramework_PACKAGE_INSTALL_DIR}/cmake")

configure_file(cmake/GrisFrameworkConfig.cmake.in         cmake/GrisFrameworkConfig.cmake @ONLY)
configure_file(cmake/GrisFrameworkConfigVersion.cmake.in  cmake/GrisFrameworkConfigVersion.cmake @ONLY)

# install Config files
install(FILES "${CMAKE_BINARY_DIR}/cmake/GrisFrameworkConfig.cmake" "${CMAKE_BINARY_DIR}/cmake/GrisFrameworkConfigVersion.cmake" DESTINATION "${GrisFramework_PACKAGE_INSTALL_DIR}")

# create the bundle.cmake file in binary tree
gris_bundle_configure_file()