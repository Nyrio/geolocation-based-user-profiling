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
void services::Core::testDJClustering(){
	data::PointSet cluster;
		//data::get_locations(1);
		data::TimeLoc tl1;
		tl1.t=0;
		tl1.loc.lat=0;
		tl1.loc.lon=0;
		cluster.insert(tl1);
		data::TimeLoc tl2;
		tl2.t=1;
		tl2.loc.lat=0.00001;
		tl2.loc.lon=0;
		cluster.insert(tl2);
		data::TimeLoc tl3;
		tl3.t=2;
		tl3.loc.lat=0.00002;
		tl3.loc.lon=0;
		cluster.insert(tl3);
		data::TimeLoc tl4;
		tl4.t=3;
		tl4.loc.lat=-0.00021;
		tl4.loc.lon=0;
		cluster.insert(tl4);
		data::TimeLoc tl5;
		tl5.t=4;
		tl5.loc.lat=-0.00022;
		tl5.loc.lon=0;
		cluster.insert(tl5);
		for(auto t:cluster){
			cout<<t.t<<" "<<t.loc.lat<<" "<<t.loc.lon<<endl;
		}
		services::DJCluster djcluster;
		djcluster.load(cluster);
		vector<Cluster*> clusters =djcluster.run(0.0002f,2);
		for(int i = 0;i<clusters.size();i++){
			cout<<"cluster:"<<endl;
			for(auto point: *clusters[i]){
				cout<<"timeloc: "<<point.t<<" "<<point.loc.lat<<" "<<point.loc.lon<<endl;
			}

		}
}
