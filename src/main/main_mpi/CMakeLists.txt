IF(WIN32)
	set(GDAL_HOME ../../../include/gdal)
	SET(LIB_DIR ../../../lib)
	INCLUDE_DIRECTORIES(../../base/util ../../base/data ../../base/bmps ../../base/module_setting ../../../include/mongo-c-driver ../main_omp ${MPI_INCLUDE} ${GDAL_HOME}/include)
	LINK_DIRECTORIES(${GDAL_HOME}/lib ${LIB_DIR} ../../../include/mongo-c-driver ${MPI_LIBDIR} ${PROJECT_BINARY_DIR})
ELSE()
	SET(CMAKE_CXX_COMPILER mpic++)
	INCLUDE_DIRECTORIES(../../base/util ../../base/data ../../base/bmps ../../base/module_setting ../../../include/mongo-c-driver ../main_omp ${MPI_HOME}/include ${GDAL_HOME}/include)
	LINK_DIRECTORIES(${GDAL_HOME}/lib /usr/lib ../../../include/mongo-c-driver ${MPI_HOME}/lib ${SQLITE3_HOME}/lib ${PROJECT_BINARY_DIR})
ENDIF(WIN32)
#ADD_DEFINITIONS(-DDEBUG_OUTPUT)

SET(SRC_LIST ReadData.cpp main.cpp ManagementProcess.cpp CalculateProcess.cpp CombineRaster.cpp)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
ADD_EXECUTABLE(seims ${SRC_LIST})
#SET(SRC_LIST ReadData.cpp ManagementProcess.cpp CombineRaster.cpp CalculateProcessMonolithic.cpp MainMonolithic.cpp)
#ADD_EXECUTABLE(simu_mono ${SRC_LIST})
IF(WIN32)
	TARGET_LINK_LIBRARIES(seims util data bmps module_setting main_mongodb ${MPI_LIB} gdal_i.lib mongoc.lib bson.lib)
	SET_PROPERTY(TARGET seims PROPERTY FOLDER "main")
ELSE()
	TARGET_LINK_LIBRARIES(seims libgdal.so libsqlite3.so libutil.so libdata.so libbmps.so libmodule_setting.so libmain_mongodb.so libbson.so libmongoc.so)
ENDIF(WIN32)


#TARGET_LINK_LIBRARIES(simu_mono libgdal.so libsqlite3.so libutil.a libdata.so libbmps.so libmodule_setting.so libmain_mongodb.so libbson.so libmongoc.so)
