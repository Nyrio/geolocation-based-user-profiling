#pragma once
#include <string>
#include "LocPoint.h"

using namespace std;

namespace data
{
	typedef struct {
		string name;
		string type;
	} Building;

	void api_query(); // placeholder

	Building* get_building(LocPoint lp, float radius = 5.0f);
}
