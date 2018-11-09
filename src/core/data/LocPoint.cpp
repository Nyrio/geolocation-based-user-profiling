#include "LocPoint.h"
#include <iostream>


data::LocPoint::LocPoint(double lat, double longi):lat(lat),longi(longi)
{
	std::cout << "Loc created at "<<lat<<" - "<<longi  << std::endl;
}


data::LocPoint::~LocPoint()
{
}