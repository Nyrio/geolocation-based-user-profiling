#include "clusters.h"
#define _USE_MATH_DEFINES
#include <iostream>
#include <set>
#include <map>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <future>
#include "external_api.h"

using namespace std;

data::LocPoint services::cluster_centroid(const data::Cluster& cluster)
{
	// It would be false to consider latitude and longitude as cartesian
	// coordinates. Instead, we project in 3D, find the centroid and project
	// back on the latitude and longitude system

	size_t c_size = cluster.size();
	double x = 0, y = 0, z = 0;

	for(auto tl: cluster)
	{
		double lat = tl.loc.lat * M_PI / 180.0;
		double lon = tl.loc.lon * M_PI / 180.0;
		x += cos(lat) * cos(lon);
		y += cos(lat) * sin(lon);
		z += sin(lat);
	}
	x = x / c_size;
	y = y / c_size;
	z = z / c_size;

	double c_lat = atan2(z, sqrt(x * x + y * y)) * 180.0 / M_PI;
	double c_lon = atan2(y, x) * 180.0 / M_PI;

	return { c_lat, c_lon };
}


double services::get_cluster_hours_spent(const data::Cluster& cluster)
{
	time_t earliest = (*cluster.begin()).t;
	time_t latest = (*cluster.rbegin()).t;
	return difftime(latest, earliest) / 3600.0;
}


data::PointSet services::reduce_precision(
	const data::PointSet& pointset, const data::WorkflowParam& wp)
{
	data::PointSet new_set;
	time_t last_time = 0;
	for(auto tl: pointset)
	{
		if(last_time == 0 || difftime(tl.t, last_time) > wp.precisionSec)
		{
			new_set.insert(tl);
			last_time = tl.t;
		}
	}
	return new_set;
}


vector<data::Cluster> services::divide_cluster(
	const data::Cluster& cluster, const data::WorkflowParam& wp)
{
	vector<data::Cluster> clusters;
	data::Cluster last_cluster;
	time_t last_time;
	for(auto tl: cluster)
	{
		if(last_cluster.size() && difftime(tl.t, last_time) / 60.0 > wp.sepMinutes)
		{
			clusters.push_back(last_cluster);
			last_cluster.clear();
		}
		last_cluster.insert(tl);
		last_time = tl.t;
	}
	if(last_cluster.size()) clusters.push_back(last_cluster);

	return clusters;
}


vector<pair<data::PointOfInterest, vector<data::Visit>>> services::clusters_visits(
		const vector<data::Cluster>& clusters, const data::WorkflowParam& wp, bool ignoreAmenities)
{
	vector<pair<data::PointOfInterest, vector<data::Visit>>> visits;
	visits.resize(clusters.size());

	// Calculate the centroid of each cluster
	// TODO: parallelize with OpenMP
	for(uint i = 0; i < clusters.size(); i++)
	{
		visits[i].first.centroid = cluster_centroid(clusters[i]);
	}

	if(!ignoreAmenities)
	{
		vector<future<set<string>>> amenities;
		// Launch asynchronously the API calls for reverse geocoding
		amenities.resize(clusters.size());
		for(uint i = 0; i < clusters.size(); i++)
		{
			amenities[i] = async(launch::async, data::get_amenities,
								visits[i].first.centroid, wp);
		}
		// Gather the responses calculated asynchronously with an external API
		for(uint i = 0; i < clusters.size(); i++)
		{
			if(amenities[i].valid()) {
				visits[i].first.amenities = amenities[i].get();
			}
			else {
				visits[i].first.amenities = set<string>();
			}
		}
	}

	// For each spatial cluster, cut it in temporal intervals and
	// add register the start and end of each occurence
	for(uint i = 0; i < clusters.size(); i++)
	{
		vector<data::Cluster> temp_clusters = divide_cluster(clusters[i], wp);
		for(data::Cluster& temp_cluster: temp_clusters)
		{
			visits[i].second.push_back(
				{temp_cluster.begin()->t, temp_cluster.rbegin()->t});
		}
	}


	return visits;
}


vector<pair<string, int>> services::most_frequent_tags(
		const vector<pair<data::PointOfInterest, vector<data::Visit>>>& visits)
{
	map<string, int> tags_freq;
	for(auto visit: visits)
	{
		for(auto amenity: visit.first.amenities)
		{
			if(tags_freq.find(amenity) == tags_freq.end())
				tags_freq.insert(pair<string, int>(amenity, 1));
			else
				tags_freq[amenity] += 1;
		}
	}

	vector<pair<string, int>> freq_tags;
	for(auto tpair: tags_freq)
	{
		freq_tags.push_back(tpair);
	}

	struct tpair_comp {
	    bool operator()(pair< string, int> const&left, 
	                    pair< string, int> const&right) const {
	        return left.second > right.second;
	    }
	};

	sort(freq_tags.begin(), freq_tags.end(), tpair_comp());

	return freq_tags;
}