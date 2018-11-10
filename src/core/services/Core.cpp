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


void services::Core::test_cluster_features()
{
	data::Cluster cluster = {
		{{ 45.769, 4.832 }, create_date(29, 4, 2018, 10, 11)},
		{{ 45.770, 4.831 }, create_date(29, 4, 2018, 10, 19)},
		{{ 45.768, 4.833 }, create_date(29, 4, 2018, 10, 20)},
		{{ 45.771, 4.834 }, create_date(29, 4, 2018, 10, 35)},
		{{ 45.769, 4.834 }, create_date(29, 4, 2018, 10, 40)},
		{{ 45.767, 4.835 }, create_date(29, 4, 2018, 10, 47)},
		{{ 45.768, 4.832 }, create_date(29, 4, 2018, 10, 53)},
		{{ 45.767, 4.831 }, create_date(29, 4, 2018, 11, 02)}
	};
	data::LocPoint lp = services::cluster_centroid(cluster);
	cout << lp.lat << " " << lp.lon << endl;

	vector<data::Cluster> clusters = divide_cluster(cluster, 10);

	for(auto subcl: clusters) {
		double hours = services::get_cluster_hours_spent(subcl);
		cout << hours << endl;
	}
}