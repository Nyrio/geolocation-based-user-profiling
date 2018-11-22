#include "Core.h"
#include <iostream>
#include <ctime>
#include <string>
#include <fstream>
#include <algorithm>

#include "datatypes.h"
#include "clusters.h"
#include "string_utils.h"
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


// e.g show-clusters 1 2014-10-08T8:00:00Z 2014-10-8T08:30:00Z
void services::Core::show_clusters(uint id, time_t t1, time_t t2)
{
	data::PointSet points;
	if(t1 == 0 && t2 == 0) // No time range set
		points = data::get_locations(id);
	else
		points = data::get_locations(id, t1, t2);

	cout << "Got " << points.size() << " points" << endl;

	services::DJCluster djcluster;
	djcluster.load(points);
	vector<Cluster> clusters = djcluster.run(wp.eps, wp.minPts);
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

// e.g benchmark clustering 1 2014-10-08T8:00:00Z 2014-10-31T8:30:00Z 10000 10
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
		vector<data::Cluster> clusters = djcluster.run(0.0002f, 2);
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

	/*points.insert({{0, 0}, 0});
	points.insert({{0.00001, 0}, 1});
	points.insert({{0.00002, 0}, 2});
	points.insert({{-0.00021, 0}, 3});
	points.insert({{-0.00022, 0}, 4});*/
	int size=0;
	ifstream f ("gps-sample");
	string line;
	if (f.is_open())
   {
     while ( getline (f,line) )
     {
       vector<string> data = split(line,'\t');
			 vector<string> date =split(data[1],' ');
			 vector<string> dateD = split(date[0],'-');
			 vector<string> dateH = split(date[1],':');

			 points.insert({{stod(data[2]),stod(data[3])}, time_utils::create_date(stod(dateD[2]),stod(dateD[1]),stod(dateD[0]), stod(dateH[0]), stod(dateH[1]), stod(dateH[2]))});
			 size++;
		 }
     f.close();
   }


	/*for(auto t:points){
		cout << t.t << " " << t.loc.lat << " " << t.loc.lon << endl;
	}*/
	services::DJCluster djcluster;
	djcluster.load(points);
	time_t start, end;
	cout<<"start benchmark: points = "<<size<<endl;
	start= time(nullptr);
	vector<Cluster> clusters =djcluster.run(0.0002f, 10);
	end= time(nullptr);
	cout<<"time clustering: "<<difftime(end,start)<<" , nb clusters: "<<clusters.size()<<endl;


	/*for(uint i = 0; i < clusters.size(); i++)
	{
		cout << "cluster:" << endl;
		for(auto point: clusters[i])
		{
			cout << "  timeloc: " << point.t << " "
			     << point.loc.lat << " " << point.loc.lon << endl;
		}
	}*/
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
