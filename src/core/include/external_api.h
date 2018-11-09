#pragma once
#include <string>

using namespace std;

namespace data
{
	typedef struct {
		string name;
		string type;
	} Building;

	void api_query(); // placeholder

	Building* get_building(float lat, float lon, float radius = 5.0f);
}
