#include "Core.h"
#include <iostream>

#include "LocPoint.h"
#include "external_api.h"
#include "rawdata.h"

using namespace std;

services::Core::Core(string parameterFile)
{
	cout << "Core created" << endl;

	// load workflow
	if(parameterFile.compare("")==0)
		wp = data::loadParam();
	else
		wp = data::loadParam(parameterFile);
}
 
 
services::Core::~Core()
{
}