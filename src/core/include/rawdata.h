#pragma once

#include "LocPoint.h"

#include <deque>

namespace data
{
	struct UserData {
		int id;
		std::deque<std::pair<LocPoint, LocPoint>> list;
	};
	void rawdata_query(int id);
}