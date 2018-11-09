#include "Core.h"
#include <iostream>

#include "LocPoint.h"
#include "rawdata.h"

services::Core::Core(std::string parameterFile)
{
	std::cout << "Core created" << std::endl;

	// load workflow
	if(parameterFile.compare("")==0)
		wp = data::loadParam();
	else
		wp = data::loadParam(parameterFile);
}
 
 
services::Core::~Core()
{
}