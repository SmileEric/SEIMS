/*----------------------------------------------------------------------
*	Purpose: 	Raster Data
*
*	Created:	Junzhi Liu
*	Date:		29-July-2012
*
*	Revision:
*   Date:
*---------------------------------------------------------------------*/

#ifndef GRID_LAYERING_RASTER_INCLUDE
#define GRID_LAYERING_RASTER_INCLUDE

#include <string>
#include "gridfs.h"

using namespace std;

struct RasterHeader
{
	int noDataValue;
	int nRows, nCols;
	double xll, yll;
	double dx;
};

int CalCompressedIndex(int n, int* mask, int noDataValue, int* compressedIndex);

void ReadArcAscii(const char* filename, RasterHeader& rs, int*& data);
void OutputArcAscii(const char* filename, RasterHeader& rs, int *data, int noDataValue);
void ReadFromMongoFloat(gridfs *gfs, const char* remoteFilename, RasterHeader& rs, float*& data);
void ReadFromMongo(gridfs *gfs, const char* remoteFilename, RasterHeader& rs, int*& data);
int WriteStringToMongoDB(gridfs *gfs, int id, const char* type, int number, const char* s);

void OutputFlowOutD8(gridfs *gfs, int id, int nRows, int nCols, int validCount, const int* dirMatrix, int noDataValue, const int* compressedIndex);
int OutputMultiFlowOut(int nRows, int nCols, int validCount,
                            const int* degreeMatrix, const int* dirMatrix, int noDataValue, const int* compressedIndex, float*& output);
int OutputFlowOutPercentageDinf(int nRows, int nCols, int validCount, const float* angle,
                       const int* degreeMatrix, const int* reverseDirMatrix, int dirNoDataValue, const int* compressedIndex, float*& pOutput);

int* GetReverseDirMatrix(const int* dirMatrix, int nRows, int nCols, int dirNoDataValue);
int* GetInDegreeMatrix(const int* dirMatrix, int nRows, int nCols, int dirNoDataValue);
string GridLayeringFromSource(int nValidGrids, const int* dirMatrix, const int* compressedIndex, int* degreeMatrix, int nRows, int nCols, int dirNoDataValue, int outputNoDataValue, int*& layeringNum);
string LayeringFromSourceD8(const char*, gridfs *gfs, int id, int nValidGrids, const int *dirMatrix, const int *compressedIndex, RasterHeader& header, int outputNoDataValue);
string LayeringFromSourceDinf(const char*, gridfs *gfs, int id, int nValidGrids, const float* angle, const int *dirMatrix, const int *compressedIndex, RasterHeader& header, int outputNoDataValue);

int* GetOutDegreeMatrix(const int* dirMatrix, int nRows, int nCols, int dirNoDataValue);
string GridLayeringFromOutlet(int nValidGrids, const int* dirMatrix, const int* compressedIndex, int* degreeMatrix, int nRows, int nCols, int dirNoDataValue, int outputNoDataValue, int*& layeringNum);
void LayeringFromOutlet(int nValidGrids, const int *dirMatrix, const int* compressedIndex, int nRows, int nCols, RasterHeader& header, int outputNoDataValue, const char* outputTxtFile, const char* outputAscFile);

void OutputLayersToMongoDB(const char* layeringTxtFile, const char* dataType, int id, gridfs* gfs);
#endif
