#include "Core.h"
#include <iostream>
#include <ctime>
#include <algorithm>

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

void services::Core::print_house(uint id, time_t t1, time_t t2)
{
	auto houses = this->find_place_hour_range(id, wp.startNight, wp.endNight, t1, t2);
	data::LocPoint centroid1 = services::cluster_centroid(houses.first);
	data::LocPoint centroid2 = services::cluster_centroid(houses.second);
	cout << "This person seems to sleep the more at : " << centroid1.lat << " "
	<< centroid1.lon << endl;
	cout << "And at : " << centroid2.lat << " " << centroid2.lon << endl;
}
void services::Core::print_work(uint id, time_t t1, time_t t2)
{
	data::Cluster work = this->find_place_hour_range(id, wp.startWork, wp.endWork, t1, t2).first;
	data::LocPoint centroid = services::cluster_centroid(work);
	cout << "This person seems to work at : " << centroid.lat << " "
	<< centroid.lon << endl;
}

// Find 2 the places where the user has been the most in a given hour-range.
// include h1, exclude h2
pair<data::Cluster,data::Cluster> services::Core::find_place_hour_range(uint id, int h1, int h2, time_t t1, time_t t2)
{
	vector<data::Cluster> clusters = this->clusterize(id, t1, t2);

	uint indexBestPlace1 = 0;
	uint indexBestPlace2 = 0;
	uint maxInstants1 = 0;
	uint maxInstants2 = 0;
	uint ite = 0;
	for(auto place : clusters)
	{	
		uint nights = 0;
		for(auto point : place)
		{
			int hour = gmtime(&(point.t))->tm_hour;
			if( (h1 > h2 && (hour >= h1 || hour < h2)) ||
				(h1 <= h2 && (hour >= h1 && hour < h2)) )
				++nights;
		}
		data::LocPoint centroid = services::cluster_centroid(place);
		cout << "A cluster : "<< nights << " " << centroid.lat << " " << centroid.lon << endl;
		if(nights > maxInstants1)
		{
			indexBestPlace2 = indexBestPlace1;
			indexBestPlace1 = ite;
			maxInstants2 = maxInstants1;
			maxInstants1 = nights;
		}
		else if(nights > maxInstants2)
		{
			indexBestPlace2 = ite;
			maxInstants2 = nights;
		}
		++ite;
	}
	pair<data::Cluster,data::Cluster> result = {clusters[indexBestPlace1], clusters[indexBestPlace2]};

	return result;
}

vector<data::Cluster> services::Core::clusterize(uint id, time_t t1, time_t t2)
{
	data::PointSet points;
	if(t1 == 0 && t2 == 0) // No time range set
		points = data::get_locations(id);
	else
		points = data::get_locations(id, t1, t2);

	cout << "Got " << points.size() << " points" << endl;

	points = services::reduce_precision(points, wp);
	cout << "Kept " << points.size() << " points" << endl;

	services::DJCluster djcluster;
	djcluster.load(points);

	vector<data::Cluster> clusters = djcluster.run(wp);
	return clusters;
}


// e.g show-clusters 1 2014-10-01T00:00:00Z 2014-11-01T00:00:00Z
void services::Core::show_clusters(uint id, time_t t1, time_t t2)
{
	vector<data::Cluster> clusters = this->clusterize(id, t1, t2);

	cout << "clusters:" << endl;
	uint pInClusters = 0;
	for(data::Cluster cluster: clusters)
	{
		data::LocPoint centroid = services::cluster_centroid(cluster);
		pInClusters += cluster.size();
		cout << "  centroid " << centroid.lat << " " << centroid.lon
		     << "\t" << cluster.size() << endl;
	}
	cout << "Points in clusters: " << pInClusters << endl;
}


/*
	Benchmarks
*/

// e.g benchmark clustering 1 2014-10-01T00:00:00Z 2014-11-01T00:00:00Z 10000 10
void services::Core::benchmark_clustering(uint id, time_t t1, time_t t2,
	                                      uint nbmax, uint nbmes)
{
	time_t start, end;

	data::PointSet points;

	start = time(nullptr);
	points = data::get_locations(1, t1, t2);
	end = time(nullptr);

	cout << "Got " << points.size() << " points in "
	     << difftime(end, start) << " s" << endl;

	points = services::reduce_precision(points, wp);
	cout << "Kept " << points.size() << " points" << endl;

	for(uint mesNum = 1; mesNum <= nbmes; mesNum++)
	{
		uint sampleSize = mesNum * nbmax / nbmes;
		if(sampleSize > points.size()) break;

		data::PointSet sample;
		uint count = 0;
		for(const data::TimeLoc& tl: points)
		{
			sample.insert(tl);
			count++;
			if(count == sampleSize) break;
		}

		services::DJCluster djcluster;
		djcluster.load(sample);
		start = time(nullptr);
		vector<data::Cluster> clusters = djcluster.run(wp);
		end = time(nullptr);
		cout << sampleSize << ": " << difftime(end, start)
		     << " s" << endl;
	}
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
	data::PointSet points;
	//data::get_locations(1);

	points.insert({{0, 0}, 0});
	points.insert({{0.00001, 0}, 1});
	points.insert({{0.00002, 0}, 2});
	points.insert({{-0.00021, 0}, 3});
	points.insert({{-0.00022, 0}, 4});

	for(auto t:points){
		cout << t.t << " " << t.loc.lat << " " << t.loc.lon << endl;
	}
	services::DJCluster djcluster;
	djcluster.load(points);
	vector<data::Cluster> clusters = djcluster.run(wp);
	for(uint i = 0; i < clusters.size(); i++)
	{
		cout << "cluster:" << endl;
		for(auto point: clusters[i])
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
