#pragma once

#include "LocPoint.h"

#include <deque>
#include <ctime>

using namespace std;

namespace data
{
	struct UserData {
		int id;
		deque<pair<LocPoint, time_t>> list;
	};
	UserData rawdata_query(int id);
	UserData rawdata_query(int id, time_t t1, time_t t2);
}