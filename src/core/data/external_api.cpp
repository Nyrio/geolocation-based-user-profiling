#include "external_api.h"
#include <iostream>
#define FMT_HEADER_ONLY
#include "fmt/format.h"
#include "httplib.h"
#include "json.hpp"
#include "LocPoint.h"

using namespace std;
using json = nlohmann::json;

void data::api_query() {
	cout << "The external API says hi" << endl;
}


data::Building* data::get_building(data::LocPoint lp, float radius)
{
	// Compose the Overpass query
	string query = fmt::format(
		"[out:json][timeout:10];"
		" way(around:{2},{0},{1})['building'~'.*'];"
		" out tags;",
		lp.lat, lp.longi, radius
	);

	string api_url = "lz4.overpass-api.de";
	httplib::Client httpclient(api_url.c_str(), 80);
	auto res = httpclient.Post("/api/interpreter", query, "text/plain");

	if(res == nullptr) {
		cout << "API query failed on " << api_url << ":" << endl << query << endl;
		return nullptr;
	}

	// cout << res->body << endl;

	json json_data = json::parse(res->body);

	Building* building = new Building;
	if(json_data["elements"].size() >= 1) {
		building->name = json_data["elements"][0]["tags"]["name"].get<std::string>();
		building->type = json_data["elements"][0]["tags"]["building"].get<std::string>();
	}
	else {
		building->name = "";
		building->type = "unknown";
	}
	
	return building;
}