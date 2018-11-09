#pragma once
#include "datatypes.h"

using namespace std;

namespace services
{
	// Return the centroid of a given points cluster
	data::LocPoint cluster_centroid(data::Cluster cluster);

	// Return all the dates of a given points cluster
	// TODO
	// multiset<time_t> get_cluster_dates(data::Cluster cluster);

	// Cut the cluster in groups corresponding to a unique visit,
	// according to a parameter stickiness
	// I'll explain more when implementing the function
	// TODO
	// vector<data::Cluster> divide_cluster(data::Cluster cluster, float stickiness);
}
