#pragma once
#include <string>
#include <ctime>
#include "api.h"

using namespace std;
#include "SimulationModule.h"

class SSM_PE:public SimulationModule
{
public:
	SSM_PE(void);
	~SSM_PE(void);
	virtual int Execute();
	virtual void SetValue(const char* key, float data);
	virtual void Set1DData(const char* key, int n, float* data);
	virtual void Get1DData(const char* key, int* n, float** data);

	bool CheckInputSize(const char* key, int n);
	bool CheckInputData(void);

private:

	int	  m_cellSize;

	float m_t0;
	float m_tsnow;
	float m_ksubli;
	float m_kblow;
	float m_lastSWE;

	float* m_PET;
	float* m_Pnet;
	float* m_SA;	
	float  m_swe;
	float  m_swe0;
	float* m_SR;
	float* m_tMin;
	float* m_tMax;

	//result
	float* m_SE;

	bool m_isInitial;

	void initalOutputs();
};

