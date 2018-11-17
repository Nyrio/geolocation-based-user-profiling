#include "Core.h"
#include <iostream>
#include <ctime>

#include "datatypes.h"
#include "clusters.h"
#include "time_utils.h"
#include "rawdata.h"
#include "dj-cluster.h"

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


/*
	Some hardcoded tests
*/

void services::Core::test_cluster_features()
{
	data::PointSet cluster =
		data::get_locations(1, time_utils::create_date(4, 10, 2014, 8, 40, 0),
		                       time_utils::create_date(4, 10, 2014, 8, 43, 0));

	data::LocPoint lp = services::cluster_centroid(cluster);
	cout << lp.lat << " " << lp.lon << endl;

	vector<data::Cluster> clusters = divide_cluster(cluster, wp);

	for(auto subcl: clusters) {
		double hours = services::get_cluster_hours_spent(subcl);
		cout << hours << endl;
	}
}


void services::Core::testDJClustering()
{
	data::PointSet cluster;
	//data::get_locations(1);

	cluster.insert({{0, 0}, 0});
	cluster.insert({{0.00001, 0}, 1});
	cluster.insert({{0.00002, 0}, 2});
	cluster.insert({{-0.00021, 0}, 3});
	cluster.insert({{-0.00022, 0}, 4});

	for(auto t:cluster){
		cout << t.t << " " << t.loc.lat << " " << t.loc.lon << endl;
	}
	services::DJCluster djcluster;
	djcluster.load(cluster);
	vector<Cluster*> clusters =djcluster.run(0.0002f, 2);
	for(uint i = 0; i < clusters.size(); i++)
	{
		cout << "cluster:" << endl;
		for(auto point: *clusters[i])
		{
			cout << "  timeloc: " << point.t << " "
			     << point.loc.lat << " " << point.loc.lon << endl;
		}
	}
}


void services::Core::test_clusters_visits()
{
	// These are not real clusters yet...

	data::PointSet cluster1 =
		data::get_locations(1, time_utils::create_date(8, 10, 2014, 7, 0, 0),
		                       time_utils::create_date(8, 10, 2014, 8, 0, 0));

	data::PointSet cluster2 =
		data::get_locations(1, time_utils::create_date(9, 10, 2014, 7, 30, 0),
		                       time_utils::create_date(9, 10, 2014, 8, 30, 0));

	vector<data::Cluster> clusters = {cluster1, cluster2};

	vector<pair<data::PointOfInterest, vector<data::Visit>>> visits
		= services::clusters_visits(clusters, wp);

	for(auto pp: visits)
	{
		for(auto am: pp.first.amenities)
		{
			cout << " " << am;
		}
		for(auto vv: pp.second)
		{
			cout << " " << difftime(vv.end, vv.start) / 3600.0;
		}
		cout << endl;
	}
}
