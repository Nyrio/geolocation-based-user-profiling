#include "clusters.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <ctime>

using namespace std;

data::LocPoint services::cluster_centroid(data::Cluster cluster)
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


double services::get_cluster_hours_spent(data::Cluster cluster)
{
	time_t earliest = (*cluster.begin()).t;
	time_t latest = (*cluster.rbegin()).t;
	return difftime(latest, earliest) / 3600.0;
}


vector<data::Cluster> services::divide_cluster(
	data::Cluster cluster, float sep_mn = 10)
{
	vector<data::Cluster> clusters;
	data::Cluster last_cluster;
	time_t last_time;
	for(auto tl: cluster)
	{
		if(last_cluster.size() && difftime(tl.t, last_time) / 60.0 > sep_mn)
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