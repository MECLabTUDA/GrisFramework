cmake_minimum_required(VERSION 2.8.9 FATAL_ERROR)

project(GrisFramework)

set(GRIS_VERSION_MAJOR "0")
set(GRIS_VERSION_MINOR "1")
set(GRIS_VERSION_PATCH "0")

set(BOOST_INCLUDE_DIR "D:/SDK/c++/boost_1_59_0" CACHE STRING "specifies the boost include directory")
set(BOOST_LIB_DIR "D:/SDK/c++/boost_1_59_0/lib32-msvc-14.0/" CACHE STRING "specifies the boost lib directory")

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


set(XERCES_INCLUDE_DIR "D:/SDK/c++/xerces-c-3.1.2/include" CACHE STRING "specifies the xerces include directory")
set(XERCES_BIN_DIR "D:/SDK/c++/xerces-c-3.1.2/bin" CACHE STRING "specifies the xerces binary directory")
set(XERCES_LIB_DIR "D:/SDK/c++/xerces-c-3.1.2/dir" CACHE STRING "specifies the xerces library directory")
set(PUGI_INCLUDE_DIR "D:/SDK/c++/pugi-1.7/include" CACHE STRING "specifies the pugi include directory")
set(PUGI_LIB_DIR "D:/SDK/c++/pugi-1.7/lib" CACHE STRING "specifies the pugi lib directory")
set(LOKI_INCLUDE_DIR "D:/SDK/c++/loki-0.1.7" CACHE STRING "specifies the loki include directory")


add_subdirectory(src)
