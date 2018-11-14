#pragma once
#include <string>
#include <ctime>
#include <set>

using namespace std;

namespace data 
{
	typedef struct {
		double lat;
		double lon;
	} LocPoint;

	typedef struct {
		LocPoint loc;
		time_t t;
	} TimeLoc;

	inline bool operator<(const TimeLoc& tl1, const TimeLoc& tl2)
	{
		if(tl1.t <= tl2.t
		   && (tl1.loc.lat < tl2.loc.lat
		   	   || (tl1.loc.lat == tl2.loc.lat && tl1.loc.lon < tl2.loc.lon)))
			return true;
		else
			return false;
	}

	using Cluster = multiset<data::TimeLoc>;

	typedef struct {
		string name;
		string type;
	} Building;
}
