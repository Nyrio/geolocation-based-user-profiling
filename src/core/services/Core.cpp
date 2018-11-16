#include "Core.h"
#include <iostream>
#include <ctime>

#include "datatypes.h"
#include "clusters.h"
#include "time_utils.h"
#include "rawdata.h"

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


void services::Core::test_cluster_features()
{
	data::PointSet cluster =
		data::get_locations(1, time_utils::create_date(4, 10, 2014, 8, 40, 0),
		                       time_utils::create_date(4, 10, 2014, 8, 43, 0));

	data::LocPoint lp = services::cluster_centroid(cluster);
	cout << lp.lat << " " << lp.lon << endl;

	vector<data::Cluster> clusters = divide_cluster(cluster, 10);

	for(auto subcl: clusters) {
		double hours = services::get_cluster_hours_spent(subcl);
		cout << hours << endl;
	}
}