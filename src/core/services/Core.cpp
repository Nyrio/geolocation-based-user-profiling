#include "Core.h"
#include <iostream>

#include "LocPoint.h"
#include "rawdata.h"

services::Core::Core()
{
	std::cout << "Core created" << std::endl;

	data::LocPoint p1(1,2);	
	data::rawdata_query(4);
}


services::Core::~Core()
{
}