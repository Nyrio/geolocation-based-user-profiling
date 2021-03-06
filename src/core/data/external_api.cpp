#include "external_api.h"
#include <iostream>
#include <vector>
#define FMT_HEADER_ONLY
#include "fmt/format.h"
#include "httplib.h"
#include "json.hpp"
#include "string_utils.h"

#define SPAM_LIMIT 20

using namespace std;
using json = nlohmann::json;

set<string> data::get_amenities(const data::LocPoint& lp, const data::WorkflowParam& wp)
{
	set<string> amenities;

	// Compose the Overpass query
	string filters = str_join(wp.placeTypes, "|");
	string query = fmt::format(
		"[out:json][timeout:10];"
		" (way(around:{2},{0},{1})[~'^({3})?$'~'.'];"
		"  node(around:{2},{0},{1})[~'^({3})?$'~'.'];);"
		" out tags;",
		lp.lat, lp.lon, wp.searchRadius, filters
	);

	vector<string> api_urls = {
		"lz4.overpass-api.de",
		"z.overpass-api.de",
		"overpass.kumi.systems"
	};

	// Try all the API servers until we get a response or reach the spam_limit
	uint itry = 0;
	while(itry < SPAM_LIMIT)
	{
		// Initialize the HTTP client for API calls
		string api_url = api_urls[itry % api_urls.size()];
		httplib::Client httpclient(api_url.c_str(), 80);

		// Call the API
		auto res = httpclient.Post("/api/interpreter", query, "text/plain");

		if(res == nullptr)
		{
			cerr << "API query failed on " << api_url << endl;
		}
		else
		{
			// Try to parse the response as JSON
			try {
				json json_data = json::parse(res->body);
				// Loop over the given nodes and ways
				for(uint i = 0; i < json_data["elements"].size(); i++)
				{
					// Try to identify relevant tags
					json tags = json_data["elements"][i]["tags"];
					for(const string& placeType: wp.placeTypes)
					{
						if(tags.find(placeType) != tags.end())
						{
							string amenity = tags[placeType].get<string>();
							if(amenity != "yes")
								amenities.insert(amenity);
						}
					}
				}
				break;
			}
			catch(...)
			{
			}
		}
		itry += 1;
	}

	return amenities;
}