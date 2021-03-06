# This file is used for make Microsoft Visual Studio Project files and compile executable files in Linux/Unix.
# This is preprocessing for SEIMS.
# First released : Liangjun Zhu
# Date           : Jun. 19, 2015
cmake_minimum_required(VERSION 2.6)
IF(COMMAND CMAKE_POLICY)
	CMAKE_POLICY(SET CMP0015 NEW)
ENDIF(COMMAND CMAKE_POLICY)
PROJECT (SEIMS_Preprocess)
#SET (CMAKE_BUILD_TYPE "Debug")
SET (CMAKE_BUILD_TYPE "Release")
IF(WIN32)
	set (MPI_INCLUDE "C:/Program Files/Microsoft HPC Pack 2012/Inc")
	set (MPI_LIBDIR "C:/Program Files/Microsoft HPC Pack 2012/Lib/i386")
	set (MPI_LIB "C:/Program Files/Microsoft HPC Pack 2012/Lib/i386/msmpi.lib")
ELSE()
	SET (CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -fopenmp -fPIC -g -ggdb")
	SET (CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -fopenmp -fPIC")
	SET (MPI_HOME /usr/lib64/openmpi)
	SET (CMAKE_CXX_COMPILER ${MPI_HOME}/bin/mpic++)
	SET (GDAL_HOME /soft/share)
	SET (MONGO_LIB_HOME /soft/share/mongo-c-driver-f06669b)
	SET (LIB_DIR /usr/lib)
ENDIF(WIN32)

SET (PREPROCESS_HOME ./cpp_src)
ADD_SUBDIRECTORY(${PREPROCESS_HOME})
IF(WIN32)
	SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
## copy external dll libraries to Debug and Release folder
	SET(EXTERNAL_DLLS "../lib/dlls/gdal15-vc9.dll" "../lib/dlls/libpng13-vc9.dll" "../lib/dlls/libexpat.dll" "../lib/dlls/zlib1-vc9.dll" "../lib/dlls/mongoc.dll" "../lib/dlls/bson.dll")
	FILE(COPY ${EXTERNAL_DLLS} DESTINATION ${PROJECT_BINARY_DIR}/Debug)
	FILE(COPY ${EXTERNAL_DLLS} DESTINATION ${PROJECT_BINARY_DIR}/Release)
ENDIF()
MESSAGE(STATUS "Preprocessing program for SEIMS is Ready for you!")