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


time_t create_date(int day, int month, int year, int hours = 0,
	               int minutes = 0, int seconds = 0)
{
	struct tm timeinfo = {
		seconds,
		minutes,
		hours,
		day,
		month - 1,
		year - 1900,
		0, // osef
		0, // osef
		-1
	};
	return mktime(&timeinfo);
}


void services::Core::do_stuff()
{
	data::Cluster cluster = {
		{{ 2, 1 }, create_date(29, 4, 2018, 10)},
		{{ 1, 358 }, create_date(28, 4, 2018)},
		{{ 1, 2 }, create_date(1, 5, 2018, 18, 30)},
		{{ -3, 359 }, create_date(30, 4, 2018)}
	};
	data::LocPoint lp = services::cluster_centroid(cluster);
	cout << lp.lat << " " << lp.lon << endl;

	double hours = services::get_cluster_hours_spent(cluster);
	cout << hours << endl;
}