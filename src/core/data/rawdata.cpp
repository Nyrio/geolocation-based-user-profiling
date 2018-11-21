#include "rawdata.h"
#include <iostream>
#include <string>
#include <map>
#define FMT_HEADER_ONLY
#include "fmt/format.h"
#include "time_utils.h"
#include "InfluxWrapper.hpp"
#include "json.hpp"

using namespace std;
using json = nlohmann::json;


data::InfluxWrapper * get_wrapper()
{
	data::InfluxWrapper * wrapper = new data::InfluxWrapper("127.0.0.1", 8086, "root", "root");
	return wrapper;
}


data::PointSet data::get_locations(uint id, string add_clause)
{
	string query = fmt::format(
		"SELECT \"time\", \"id\", \"lat\", \"lon\""
		" FROM gps WHERE \"id\" = '{0}' {1};",
		id, add_clause
	);
	InfluxWrapper * wrapper = get_wrapper();
	json resp = wrapper->query(query);

	data::PointSet ps;
	for(json location: resp["results"][0]["series"][0]["values"])
	{
		TimeLoc tl;
		tl.t = time_utils::from_rfc3339(location[0].get<string>());
		tl.loc.lat = location[2].get<double>();
		tl.loc.lon = location[3].get<double>();
		ps.insert(tl);
	}

	delete wrapper;
	return ps;
}

data::PointSet data::get_locations(uint id, time_t t1, time_t t2)
{
	string clause = fmt::format(
		"AND time > '{0}' AND time < '{1}'",
		time_utils::to_rfc3339(t1), time_utils::to_rfc3339(t2)
	);
	return data::get_locations(id, clause);
}
