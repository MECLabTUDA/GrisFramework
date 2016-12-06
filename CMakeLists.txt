cmake_minimum_required(VERSION 2.8.9 FATAL_ERROR)

project(GrisFramework)

# generate Version variables
include(cmake/version.cmake)

# define _gris_deploy_after_build
include(cmake/DeployAfter.cmake)
# define _gris_configure_resource_rc
include(cmake/ConfigureResource.cmake)
# define _gris_bundle_target
include(cmake/BundleTarget.cmake)

include(cmake/TargetAppendVersion.cmake)

set(SDK_BASEPATH CACHE PATH "specifies the sdk base path")

if (SDK_BASEPATH STREQUAL "")
  message(SEND_ERROR "Set SDK_BASEPATH to the directory containing the 3rd party libraries")
endif()

option(BUILD_Testing "enable tests" OFF)

set(GrisFramework_BIN_INSTALL_DIR "bin")
set(GrisFramework_LIB_INSTALL_DIR "lib")
set(GrisFramework_PACKAGE_INSTALL_DIR "lib/cmake/GrisFramework-${GrisFramework_SHORT_VERSION}")
set(GrisFramework_Testing_BIN_INSTALL_DIR "tests")
set(GrisFramework_Testing_LIB_INSTALL_DIR "tests/lib")

set(BOOST_INCLUDE_DIR "${SDK_BASEPATH}/boost_1_59_0/"                 CACHE STRING "specifies the boost include directory")
set(BOOST_LIB_DIR     "${SDK_BASEPATH}/boost_1_59_0/lib32-msvc-14.0/" CACHE STRING "specifies the boost lib directory")

#set(BOOST_CHRONO_R "boost_chrono-vc140-mt-1_59.lib" CACHE string "")
#set(BOOST_DATE_TIME_R "boost_date_time-vc140-mt-1_59.lib" CACHE string "")
set(BOOST_FILESYSTEM_R "libboost_filesystem-vc140-mt-s-1_59.lib" CACHE string "")
#set(BOOST_PROGRAM_OPTIONS_R "boost_program_options-vc140-mt-1_59.lib" CACHE string "")
#set(BOOST_SERIALIZATION_R "boost_serialization-vc140-mt-1_59.lib" CACHE string "")
#set(BOOST_SYSTEM_R "boost_system-vc140-mt-1_59.lib" CACHE string "")
#set(BOOST_THREAD_R "boost_thread-vc140-mt-1_59.lib" CACHE string "")
#set(BOOST_UNIT_TEST_R "boost_unit_test_framework-vc140-mt-1_59.lib" CACHE string "")

set(BOOST_LIBS 
   ${BOOST_CHRONO_R} ${BOOST_DATE_TIME_R} ${BOOST_FILESYSTEM_R}
	 ${BOOST_PROGRAM_OPTIONS_R} ${BOOST_SERIALIZATION_R} ${BOOST_SYSTEM_R}
	 ${BOOST_THREAD_R} ${BOOST_UNIT_TEST_R})


set(XERCES_INCLUDE_DIR "${SDK_BASEPATH}/xerces-c-3.1.2/include" CACHE STRING "specifies the xerces include directory")
set(XERCES_BIN_DIR "${SDK_BASEPATH}/xerces-c-3.1.2/bin" CACHE STRING "specifies the xerces binary directory")
set(XERCES_LIB_DIR "${SDK_BASEPATH}/xerces-c-3.1.2/dir" CACHE STRING "specifies the xerces library directory")
set(PUGI_INCLUDE_DIR "${SDK_BASEPATH}/pugixml-1.7/include" CACHE STRING "specifies the pugi include directory")
set(PUGI_LIB_DIR "${SDK_BASEPATH}/pugixml-1.7/lib" CACHE STRING "specifies the pugi lib directory")
set(LOKI_INCLUDE_DIR "${SDK_BASEPATH}/loki-0.1.7" CACHE STRING "specifies the loki include directory")


add_subdirectory(src)

file(GLOB GrisFramework_MACROS_LIST RELATIVE cmake "cmake/*.cmake")
# cmake files not to install/include
list(REMOVE_ITEM GrisFramework_MACROS_LIST version.cmake)
set(GrisFramework_MACROS_LIST_FULL )
foreach(_file IN LISTS ${GrisFramework_MACROS_LIST})
  set(GrisFramework_MACROS_LIST_FULL ${GrisFramework_MACROS_LIST_FULL} "cmake/${_file}")
endforeach()

configure_file(cmake/GrisFrameworkConfig.cmake.in         GrisFrameworkConfig.cmake @ONLY)
configure_file(cmake/GrisFrameworkConfigVersion.cmake.in  GrisFrameworkConfigVersion.cmake @ONLY)

# install
install(FILES ${GrisFramework_MACROS_LIST_FULL} DESTINATION ${GrisFramework_PACKAGE_INSTALL_DIR})
configure_file(cmake/GrisFrameworkConfig.cmake.in         cmake/GrisFrameworkConfig.cmake @ONLY)
configure_file(cmake/GrisFrameworkConfigVersion.cmake.in  cmake/GrisFrameworkConfigVersion.cmake @ONLY)

# create the bundle.cmake file in binary tree
_gris_configure_bundle_file()