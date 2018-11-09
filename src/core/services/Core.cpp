#include "Core.h"
#include <iostream>

#include "LocPoint.h"
#include "external_api.h"
#include "rawdata.h"
#include "workflow.h"

using namespace std;

services::Core::Core()
{
	std::cout << "Core created" << std::endl;
	
	data::WorkflowParam w = data::loadParam();
}


services::Core::~Core()
{
}