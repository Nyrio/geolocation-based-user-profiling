#pragma once
#include <ctime>
#include <vector>
#include "datatypes.h"

using namespace std;

namespace services
{
	// Return the centroid of a given points cluster
	data::LocPoint cluster_centroid(data::Cluster cluster);

	// Return the time spent in a cluster (given that the points correspond to
	// a unique visit)
	double get_cluster_hours_spent(data::Cluster cluster);

	// Cut the cluster in groups corresponding to a unique visit, with the
	// criteria that points in the same visit mustn't be separated with
	// more than sep_mn minutes
	vector<data::Cluster> divide_cluster(data::Cluster cluster, float sep_mn);
}
