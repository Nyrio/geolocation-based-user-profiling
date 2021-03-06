#pragma once
#include <ctime>
#include <vector>
#include "datatypes.h"
#include "workflow.h"

namespace services
{
	// Return the centroid of a given points cluster
	data::LocPoint cluster_centroid(const data::Cluster& cluster);

	// Return the time spent in a cluster (given that the points correspond to
	// a unique visit)
	double get_cluster_hours_spent(const data::Cluster& cluster);

	// Reduce the number of points by using a minimum time interval between two points
	data::PointSet reduce_precision(
		const data::PointSet& pointset, const data::WorkflowParam& wp);

	// Cut the cluster in groups corresponding to a unique visit, with the
	// criteria that points in the same visit mustn't be separated with
	// more than sep_mn minutes
	vector<data::Cluster> divide_cluster(const data::Cluster& cluster,
		                                 const data::WorkflowParam& wp);

	// TODO: write what this function is doing
	vector<pair<data::PointOfInterest, vector<data::Visit>>> clusters_visits(
		const vector<data::Cluster>& clusters, const data::WorkflowParam& wp,
		bool ignoreAmenities = false);

	// Sort tags by most frequent
	vector<pair<string, int>> most_frequent_tags(
		const vector<pair<data::PointOfInterest, vector<data::Visit>>>& visits);
}
