#pragma once
#include <string>
#include "datatypes.h"

using namespace std;

namespace data
{
	void api_query(); // placeholder

	Building get_building(LocPoint lp, float radius = 5.0f);
}
