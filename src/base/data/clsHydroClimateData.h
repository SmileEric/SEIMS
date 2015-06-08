#pragma once

#include <string>
#include <map>
#include "bson.h"


using namespace std;

class clsHydroClimateData
{
public:
	clsHydroClimateData(string databasePath,string tableName);
	clsHydroClimateData(string databasePath,int id,string type);
	~clsHydroClimateData(void);

	void loadTimeSeriesData(time_t startTime,time_t endTime,int interval);
	float getTimeSeriesData(time_t time);

	static bool IsHydroClimateDataType(const string* type);
	static string getTableNameFromIDAndType(string databasePath,int id,string type);

	void dump(ostream* fs);

	// for mongodb
	clsHydroClimateData();
	void appendData(time_t t, float value)
	{
		m_data.insert(map<time_t, float>::value_type(t, value));
	}
private:
	void readData();
private:
	string	m_Units;
	time_t	m_StartDate;
	time_t	m_EndDate;
	int		m_Interval;
	string	m_TableName;
	string  m_Type;

	map<time_t,float> m_data;
private:
	string	m_databasePath;
};
