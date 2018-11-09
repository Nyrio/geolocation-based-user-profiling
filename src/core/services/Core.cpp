#include "Core.h"
#include <iostream>

#include "LocPoint.h"
#include "rawdata.h"
#include "workflow.h"

services::Core::Core()
{
	std::cout << "Core created" << std::endl;
	
	data::WorkflowParam w = data::loadParam();
}


services::Core::~Core()
{
}