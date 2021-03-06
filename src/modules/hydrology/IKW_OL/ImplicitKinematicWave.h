/*----------------------------------------------------------------------
*	Purpose: 	Overland routing using 4-point implicit finite difference method
*
*	Created:	Junzhi Liu
*	Date:		23-Febrary-2011
*
*	Revision:
*   Date:
*---------------------------------------------------------------------*/

#pragma once

#include <string>
#include <ctime>
#include <cmath>
#include <map>
#include "SimulationModule.h"
using namespace std;

#define MIN_FLUX 1e-12f /// \def minimum flux (m3/s) in kinematic wave
#define MAX_ITERS 10

class ImplicitKinematicWave : public SimulationModule
{
public:
	ImplicitKinematicWave(void);
	~ImplicitKinematicWave(void);

	virtual int Execute();

	virtual void SetValue(const char* key, float data);
	virtual void GetValue(const char* key, float* data);
	virtual void Set1DData(const char* key, int n, float* data);
	virtual void Get1DData(const char* key, int* n, float** data);
	virtual void Set2DData(const char* key, int nrows, int ncols, float** data);

	bool CheckInputSize(const char* key, int n);
	bool CheckInputData(void);

private:
	/// deal with positive and negative float numbers
	float Power(float a, float n)
	{
		if (a >= 0)
			return pow(a, n);
		else
			return -pow(-a, n);
	}

	float GetNewQ(float qIn, float qLast, float surplus, float alpha, float dt, float dx);

	void OverlandFlow(int id);

	void initalOutputs();

	/// size
	int m_size;

	/// cell width of the grid (m)
	float m_dx;
	/// time step (second)
	float m_dtStorm;

	/// slope (percent)
	float *m_s0;
	/// manning's roughness
	float *m_n;
	/// channel width (zero for non-channel cells)
	float *m_chWidth;

	/**
	*	@brief flow direction by the rule of ArcGIS
	*
	*	The value of direction is as following:
		32 64 128
		64     1
		8   4  2
	*/
	float *m_direction;
	/**
	*	@brief 2d array of flow in cells
	*
	*	The first element in each sub-array is the number of flow in cells in this sub-array
	*/
	float **m_flowInIndex;

	/// flow out index
	float *m_flowOutIndex;

	/**
	*	@brief Routing layers according to the flow direction
	*
	*	There are not flow relationships within each layer.
	*	The first element in each layer is the number of cells in the layer
	*/
	float **m_routingLayers;
	int m_nLayers;

	/// water height available for runoff (surface runoff)
	float *m_sr;
	/// discharge to the downslope cell
	float *m_q;
	/// flow velocity 
	float *m_vel;

	/// id of outlet
	int m_idOutlet;

	/////////////////////////////////////////////////////////////////////////
	// reinfiltration
	// if pNet > infilPotential, surplus = 0
	// else surplus = infilPotential - pNet
	float *m_infilCapacitySurplus;
	// twice infiltration
	/// cumulative infiltration depth (m)
	float *m_accumuDepth;
	/// infiltration map of watershed (mm) of the total nCells
	float* m_infil;
	/// reinfiltration
	float* m_reInfil;

	//////////////////////////////////////////////////////////////////////////
	// the following are intermediate variables 
	/**
	*	@brief convert direction code to whether diagonal
	*
	*	derived from flow direction
		1  0  1
		0     0
		1  0  1
	*/
	std::map<int, int> m_diagonal;

	/// flow width of each cell
	float *m_flowWidth;
	/// stream link
	float *m_streamLink;
	/// flow length of each cell
	float *m_flowLen;
	/// alpha in manning equation
	float *m_alpha;
	/// sin value of slope
	float *m_sRadian;

	/// sqrt(2.0f)
	float SQ2;
};

