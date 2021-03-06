#include "IUH_OL.h"
#include "MetadataInfo.h"
#include "ModelException.h"
#include "util.h"
#include <map>
#include <omp.h>

using namespace std;

IUH_OL::IUH_OL(void): m_TimeStep(-1), m_cellSize(-1), m_CellWidth(NODATA), m_nsub(-1), m_subbasin(NULL),
	m_iuhCell(NULL), m_rs(NULL), m_iuhCols(-1),m_cellFlowCols(-1), m_Q_SBOF(NULL), m_cellFlow(NULL)
{
}

IUH_OL::~IUH_OL(void)
{
	//// cleanup
	if(m_Q_SBOF!=NULL)
		delete [] m_Q_SBOF;
	if(this->m_cellFlow != NULL)
	{
		for(int i=0;i<this->m_cellSize;i++)
		{
			if(this->m_cellFlow[i] != NULL) delete [] this->m_cellFlow[i];
		}
		delete [] this->m_cellFlow;
	}
}

bool IUH_OL::CheckInputData(void)
{
	if (m_cellSize < 0)
	{
		throw ModelException("IUH_OL","CheckInputData","The parameter: m_cellSize has not been set.");
		return false;
	}
	if (FloatEqual(m_CellWidth, NODATA))
	{
		throw ModelException("IUH_OL","CheckInputData","The parameter: m_CellWidth has not been set.");
		return false;
	}
	if (m_TimeStep <= 0)
	{
		throw ModelException("IUH_OL","CheckInputData","The parameter: m_TimeStep has not been set.");
		return false;
	}

	if (m_subbasin == NULL)
	{
		throw ModelException("IUH_OL","CheckInputData","The parameter: m_subbasin has not been set.");
		return false;
	}
	//if (m_uhmaxCell == NULL)
	//{
	//	throw ModelException("IUH_OL","CheckInputData","The parameter: m_uhmax has not been set.");
	//	return false;
	//}
	//if (m_uhminCell == NULL)
	//{
	//	throw ModelException("IUH_OL","CheckInputData","The parameter: m_uhmin has not been set.");
	//	return false;
	//}
	if (m_iuhCell == NULL)
	{
		throw ModelException("IUH_OL","CheckInputData","The parameter: m_iuhCell has not been set.");
		return false;
	}
	if (m_rs == NULL)
	{
		throw ModelException("IUH_OL","CheckInputData","The parameter: m_rs has not been set.");
		return false;
	}
	if (m_date < 0)
	{
		throw ModelException("IUH_OL","CheckInputData","The parameter: m_date has not been set.");
		return false;
	}

	return true;
}

void IUH_OL::initalOutputs()
{
	if(this->m_cellSize <= 0 || this->m_subbasin == NULL)				
		throw ModelException("IUH_OL","CheckInputData","The dimension of the input data can not be less than zero.");
	// allocate the output variables

	if(m_nsub <= 0)
	{
		map<int,int> subs;
		for(int i=0;i<this->m_cellSize;i++)
		{
			subs[int(this->m_subbasin[i])] += 1;
		}
		this->m_nsub = subs.size();
	}

	//initial some variables
	if(this->m_Q_SBOF == NULL) 
	{
		m_Q_SBOF = new float[m_nsub+1];
		for (int i = 0; i <= m_nsub; i++)
		{
			m_Q_SBOF[i] = 0.f;
		}

		m_cellFlow = new float*[m_cellSize];

		for(int i=0; i<m_cellSize; i++)
			m_cellFlowCols = max(int(m_iuhCell[i][1]+1), m_cellFlowCols);
		//get m_cellFlowCols, i.e. the maximum of second column of iuh add 1.
		#pragma omp parallel for
		for(int i=0; i<m_cellSize; i++)
		{
			m_cellFlow[i] = new float[m_cellFlowCols];
			for(int j=0; j<m_cellFlowCols; j++)
				m_cellFlow[i][j] = 0.0f;
		}		
	}
}

int IUH_OL::Execute()
{
	this->CheckInputData();

	this->initalOutputs();

	#pragma omp parallel for
	for (int n = 0; n<m_nsub+1; n++)
	{
		m_Q_SBOF[n] = 0.0f;     // delete value of last time step
	}

	int nt = 0;
	float qs_cell = 0.0f;
	float area = m_CellWidth * m_CellWidth;

	#pragma omp parallel for
	for (int i = 0; i < m_cellSize; i++)
	{
		//forward one time step
		for(int j=0; j<m_cellFlowCols; j++)
		{
			if(j != m_cellFlowCols-1)	
				m_cellFlow[i][j] = m_cellFlow[i][j+1];
			else							
				m_cellFlow[i][j] = 0.0f;				
		}

		
		float v_rs = m_rs[i];
		if (v_rs > 0.0f)
		{
			int min = int(this->m_iuhCell[i][0]);
			int max = int(this->m_iuhCell[i][1]);
			int col = 2;
			for(int k=min;k<=max;k++)
			{
				m_cellFlow[i][k] += v_rs/1000.0f * m_iuhCell[i][col] * area/m_TimeStep;
				col++;
			}

		}
	}

	for (int i = 0; i < m_cellSize; i++)
	{
		//add today's flow
		int subi = (int)m_subbasin[i];
		if (m_nsub == 1)
		{
			subi = 1;
		}
		else if(subi >= m_nsub+1) 
		{
			ostringstream oss;
			oss << subi;
			throw ModelException("IUH_OL","Execute","The subbasin "+oss.str()+" is invalid.");
		}
		m_Q_SBOF[subi] += m_cellFlow[i][0];	//get new value
	}

	float tmp = 0.f;
	#pragma omp parallel for reduction(+:tmp)
	for (int n = 1; n<m_nsub+1; n++)
	{
		tmp += m_Q_SBOF[n];	    //get overland flow routing for entire watershed. 
	}
	m_Q_SBOF[0] = tmp;
	
	return 0;
}

bool IUH_OL::CheckInputSize(const char* key, int n)
{
	if(n<=0)
	{
		throw ModelException("IUH_OL","CheckInputSize","Input data for "+string(key) +" is invalid. The size could not be less than zero.");
		return false;
	}
	if(this->m_cellSize != n)
	{
		if(this->m_cellSize <=0) this->m_cellSize = n;
		else
		{
			throw ModelException("IUH_OL","CheckInputSize","Input data for "+string(key) +" is invalid. All the input data should have same size.");
			return false;
		}
	}

	return true;
}

void IUH_OL::SetValue(const char* key, float value)
{
	string sk(key);

	if (StringMatch(sk, "TimeStep"))
	{
		m_TimeStep =(int) value;
	}
	//else if (StringMatch(sk, "nCells"))
	//{
	//	m_cellSize =(int) value;
	//}
	else if (StringMatch(sk, "CellWidth"))
	{
		m_CellWidth = value;
	}
	else if (StringMatch(sk, "ThreadNum"))
	{
		omp_set_num_threads((int)value);
	}
	else									throw ModelException("IUH_OL","SetValue","Parameter " + sk 
		+ " does not exist in IUH_OL method. Please contact the module developer.");
	
}

void IUH_OL::Set1DData(const char* key, int n, float* data)
{

	this->CheckInputSize(key,n);

	//set the value
	string sk(key);
	if (StringMatch(sk,"subbasin"))
	{
		m_subbasin = data;
	}
	//else if (StringMatch(sk,"uhminCell"))
	//{
	//	m_uhminCell = data;
	//}
	//else if (StringMatch(sk, "uhmaxCell"))
	//{
	//	m_uhmaxCell = data;
	//}
	else if (StringMatch(sk, "D_SURU"))
	{
		m_rs = data;
	}
	else									throw ModelException("IUH_OL","SetValue","Parameter " + sk + 
		" does not exist in IUH_OL method. Please contact the module developer.");

}

void IUH_OL::Set2DData(const char* key, int nRows, int nCols, float** data)
{
	
	string sk(key);
	if (StringMatch(sk,"Ol_iuh"))
	{
		this->CheckInputSize("Ol_iuh",nRows);

		m_iuhCell = data;
		m_iuhCols = nCols;
	}
	else									throw ModelException("IUH_OL","SetValue","Parameter " + sk + 
		" does not exist in IUH_OL method. Please contact the module developer.");
}

void IUH_OL::Get1DData(const char* key, int* n, float** data)
{
	string sk(key);
	if (StringMatch(sk, "SBOF"))  
	{
		*data = this->m_Q_SBOF;
	}
	else									throw ModelException("IUH_OL","getResult","Result " + sk + " does not exist in IUH_OL method. Please contact the module developer.");

	*n = this->m_nsub+1;
}


