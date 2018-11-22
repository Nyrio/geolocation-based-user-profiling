#pragma once

#include "datatypes.h"

#include <ctime>
#include <map>

namespace data
{
	data::PointSet get_locations(uint id, std::string add_clause = "");
	data::PointSet get_locations(uint id, time_t t1, time_t t2);
}