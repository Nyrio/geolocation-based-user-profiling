#include "Core.h"
#include <iostream>
#include <ctime>

#include "datatypes.h"
#include "clusters.h"

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


void services::Core::do_stuff()
{
	data::Cluster cluster = {
		{{ 2, 1 }, time(nullptr)},
		{{ 1, 358 }, time(nullptr)},
		{{ 1, 2 }, time(nullptr)},
		{{ -3, 359 }, time(nullptr)}
	};
	data::LocPoint lp = services::cluster_centroid(cluster);
	cout << lp.lat << " " << lp.lon << endl;
}