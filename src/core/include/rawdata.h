#pragma once

#include "datatypes.h"

#include <ctime>
#include <map>


using namespace std;

namespace data
{
	data::PointSet get_locations(uint id, string add_clause = "");
	data::PointSet get_locations(uint id, time_t t1, time_t t2);
}