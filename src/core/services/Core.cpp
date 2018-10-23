#include "Core.h"
#include <iostream>
#include "databases.h"
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
	data::db_query();
}