cmake_minimum_required(VERSION 3.13)
project(float_crystal)

set(CMAKE_CXX_STANDARD 14)
add_definitions(-D BOOST_PYTHON_STATIC_LIB)
set(CMAKE_CXX_FLAGS "-O3 -fPIC")
set(PYTHON_ROOT "/home/zy/package//py3_5")
set(PYTHON_INCLUDEDIR "/home/zy/package/py3_5/include/python3.5m")
set(PYTHON_LIBRARYDIR "/home/zy/package/py3_5/lib")
include_directories(${PYTHON_INCLUDEDIR})
link_directories(${PYTHON_LIBRARYDIR})
#find_package(PYTHON)
#find_package(PythonLibs 3.5 REQUIRED)
#include_directories(${PYTHON_INCLUDE_DIR})
#if(PYTHON_FOUND)
#    include_directories(${Python_INCLUDEDIR})
#    #target_link_libraries(pycystal ${PYTHON_LIBRARIES})
#
#    MESSAGE( STATUS "Python_INCLUDE_DIRS = ${PYTHON_INCLUDE_DIRS}.")
#    MESSAGE( STATUS "Python_LIBRARIES = ${PYTHON_LIBRARIES}.")
#    MESSAGE( STATUS "Python_LIB_VERSION = ${PYTHON_LIB_VERSION}.")
#
#endif()


set(BOOST_ROOT "/home/zy/package/boost_1_68_0/")
set(BOOST_INCLUDEDIR "/home/zy/package/boost_1_68_0/")
set(BOOST_LIBRARYDIR "/home/zy/package/boost_1_68_0/stage/lib/")
include_directories(${BOOST_INCLUDEDIR})
link_directories(${BOOST_LIBRARYDIR})
#find_package(Boost)
#if(Boost_FOUND)
#
    #target_link_libraries(float_crystal ${Boost_LIBRARIES})
#    MESSAGE( STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}.")
#    MESSAGE( STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}.")
#    MESSAGE( STATUS "Boost_LIB_VERSION = ${Boost_LIB_VERSION}.")

#endif()


add_library(float_crystal SHARED
		python/pycrystal.cpp
		python/pyroom.cpp
		cpp/polymer.cpp
		cpp/polymer.h
		cpp/point.h
		cpp/room.cpp
		cpp/room.h
		cpp/utils.h
		)

set_target_properties(float_crystal
	PROPERTIES
	PREFIX ""      
	SUFFIX ".so")

target_link_libraries(float_crystal boost_python35)
target_link_libraries(float_crystal python3.5m)
SET(LIBRARY_OUTPUT_PATH ./python)
