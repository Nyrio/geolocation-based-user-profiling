#include "Core.h"
#include <iostream>
#include "cachedata.h"
#include "rawdata.h"
#include "external_api.h"


services::Core::Core()
{
	std::cout << "Core created" << std::endl;
}


services::Core::~Core()
{
}

void services::Core::do_stuff()
{
	data::api_query();
	data::cache_query();
	data::rawdata_query();
}