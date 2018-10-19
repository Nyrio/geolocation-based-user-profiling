#include "stdafx.h"
#include "Core.h"
#include "Terminal.h"
#include <iostream>


services::Core::Core()
{
	std::cout << "Core created" << std::endl;
	application::Terminal t;
}


services::Core::~Core()
{
}
