LINK_DIRECTORIES(${PROJECT_BINARY_DIR})
INCLUDE_DIRECTORIES(../../../base/util ../../../base/module_setting)
SET(SRC_LIST ../../../base/util/SimulationModule.cpp SNO_WB.cpp subbasin.cpp api.cpp)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
ADD_LIBRARY(SNO_WB SHARED ${SRC_LIST})
#TARGET_LINK_LIBRARIES(SNO_WB libutil.a)
TARGET_LINK_LIBRARIES(SNO_WB util module_setting)
SET_TARGET_PROPERTIES( SNO_WB PROPERTIES COMPILE_FLAGS "/openmp")
SET_PROPERTY(TARGET SNO_WB PROPERTY FOLDER "modules/hydrology_longterm")