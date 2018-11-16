#pragma once

#include "datatypes.h"

#include <ctime>
#include <map>


using namespace std;

namespace data
{
	data::PointSet get_locations(int id, string add_clause = "");
	data::PointSet get_locations(int id, time_t t1, time_t t2);
}