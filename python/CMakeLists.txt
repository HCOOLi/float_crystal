cmake_minimum_required(VERSION 3.13)
project(float_crystal)

set(CMAKE_CXX_STANDARD 14)
add_definitions(-D BOOST_PYTHON_STATIC_LIB)
set(CMAKE_CXX_FLAGS "-O2")
set(PYTHON_ROOT "C:\\Users\\1007\\Anaconda3")
set(PYTHON_INCLUDEDIR "C:\\Users\\1007\\Anaconda3\\include")
set(PYTHON_LIBRARYDIR "C:\\Users\\1007\\Anaconda3\\libs")
include_directories(${PYTHON_INCLUDEDIR})
link_directories(${PYTHON_LIBRARYDIR})
#find_package(Python)
#if(Python_FOUND)
#    include_directories(${Python_INCLUDEDIR})
#    #target_link_libraries(pycystal ${PYTHON_LIBRARIES})
#
#    MESSAGE( STATUS "Python_INCLUDE_DIRS = ${Python_INCLUDEDIR}.")
#    MESSAGE( STATUS "Python_LIBRARIES = ${Python_LIBRARIES}.")
#    MESSAGE( STATUS "Python_LIB_VERSION = ${Python_LIB_VERSION}.")
#
#endif()


set(BOOST_ROOT "D:\\boost_1_68_0\\boost_1_68_0")
set(BOOST_INCLUDEDIR "D:\\boost_1_68_0\\boost_1_68_0")
set(BOOST_LIBRARYDIR "D:\\boost_1_68_0\\boost_1_68_0\\stage\\lib")
include_directories(${BOOST_INCLUDEDIR})
link_directories(${BOOST_LIBRARYDIR})
#find_package(Boost)
#if(Boost_FOUND)
#
#    #target_link_libraries(pycystal ${Boost_LIBRARIES})
##
##    MESSAGE( STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}.")
##    MESSAGE( STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}.")
##    MESSAGE( STATUS "Boost_LIB_VERSION = ${Boost_LIB_VERSION}.")
#
#endif()


add_library(float_crystal SHARED
        pycrystal.cpp
        pyroom.cpp
        ../cpp/polymer.cpp
        ../cpp/polymer.h
        ../cpp/point.h
        ../cpp/room.cpp

        )
set_target_properties(
        float_crystal
        PROPERTIES
        PREFIX ""
        SUFFIX ".pyd"
)