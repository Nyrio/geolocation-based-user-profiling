#include "external_api.h"
#include <iostream>
#define FMT_HEADER_ONLY
#include "fmt/format.h"
#include "httplib.h"
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

data::Building data::get_building(data::LocPoint lp, float radius)
{
	Building building = {"", "unassigned"};

	// Compose the Overpass query
	string query = fmt::format(
		"[out:json][timeout:10];"
		" way(around:{2},{0},{1})['building'~'.*'];"
		" out tags;",
		lp.lat, lp.lon, radius
	);

	string api_url = "lz4.overpass-api.de";
	httplib::Client httpclient(api_url.c_str(), 80);
	auto res = httpclient.Post("/api/interpreter", query, "text/plain");

	if(res == nullptr) {
		cout << "API query failed on " << api_url << ":" << endl << query << endl;
		building = {"", "error"};
		return building;
	}

	// DEBUG
	// cout << res->body << endl;

	json json_data = json::parse(res->body);

	for(unsigned int i = 0; i < json_data["elements"].size(); i++)
	{
		json tags = json_data["elements"][i]["tags"];
		building.type = tags["building"].get<string>();
		if(tags.find("name") != tags.end())
			building.name = tags["name"].get<string>();
		else
			building.name = "";
		if(building.type != "yes")
			return building;
	}
	
	building = {"", "unknown"};
	return building;
}