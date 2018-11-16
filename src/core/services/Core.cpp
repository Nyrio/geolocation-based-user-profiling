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

void services::Core::test_clusters_visits()
{
	data::PointSet cluster1 =
		data::get_locations(1, time_utils::create_date(4, 10, 2014, 9, 0, 0),
		                       time_utils::create_date(4, 10, 2014, 10, 0, 0));

	data::PointSet cluster2 =
		data::get_locations(1, time_utils::create_date(4, 10, 2014, 10, 30, 0),
		                       time_utils::create_date(4, 10, 2014, 11, 30, 0));

	vector<data::Cluster> clusters = {cluster1, cluster2};

	vector<pair<data::PointOfInterest, vector<data::Visit>>> visits
		= services::clusters_visits(clusters, 10.0, 10.0);

	cout << "ahoy" << endl;

	for(auto pp: visits)
	{
		cout << pp.first.building.type;
		for(auto vv: pp.second)
		{
			cout << " " << difftime(vv.end, vv.start) / 3600.0;
		}
		cout << endl;
	}
}