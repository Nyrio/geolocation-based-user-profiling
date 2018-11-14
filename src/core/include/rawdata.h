#pragma once

#include "datatypes.h"

#include <ctime>
#include <map>


using namespace std;

namespace data
{
	map<int, data::PointSet> rawdata_query(int id);
	map<int, data::PointSet> rawdata_query(int id, time_t t1, time_t t2);
}