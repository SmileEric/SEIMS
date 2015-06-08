#include <stdio.h>
#include <string>
#include "api.h"
#include "util.h"
#include "AtmosphericDeposition.h"
#include <iostream>
#include "SimulationModule.h"
#include "MetadataInfo.h"
#include "MetadataInfoConst.h"

extern "C" SEIMS_MODULE_API SimulationModule* GetInstance()
{
	return new AtmosphericDeposition();
}

// function to return the XML Metadata document string
extern "C" SEIMS_MODULE_API const char* MetadataInformation()
{
	string res = "";
	MetadataInfo mdi;

	// set the information properties
	mdi.SetAuthor("Wang Lin");
	mdi.SetClass("AtmosphericDeposition", "Atmospheric Deposition.");
	mdi.SetDescription("Atmospheric Deposition.");
	mdi.SetEmail("");
	mdi.SetHelpfile("AtmosphericDeposition.chm");
	mdi.SetID("AtmosphericDeposition");
	mdi.SetName("AtmosphericDeposition");
	mdi.SetVersion("0.1");
	mdi.SetWebsite("http://www.website.com");

	mdi.AddParameter("RootDepth", "mm", "depth from the soil surface", "file.in", DT_Raster); 
	mdi.AddParameter("ConRainNitra", "mg N/L", "concentration of nitrate in the rain", "file.in", DT_Single); 
	mdi.AddParameter("ConRainAmm", "mg N/L", "concentration of ammonia in the rain", "file.in", DT_Single);

	mdi.AddInput("D_P","mm","precipitation","Module",DT_Raster);

	mdi.AddOutput("Nitrate", "kg N/ha", "amount of nitrate", DT_Array2D);
	mdi.AddOutput("Ammon", "kg N/ha", "ammonium pool for soil nitrogen", DT_Array2D);

	mdi.AddOutput("AddRainNitra", "kg N/ha", "nitrate added by rainfall", DT_Raster);
	mdi.AddOutput("AddRainAmm", "kg N/ha", "ammonium added by rainfall", DT_Raster); 

	res = mdi.GetXMLDocument();

	char* tmp = new char[res.size()+1];
	strprintf(tmp, res.size()+1, "%s", res.c_str());
	return tmp;
}