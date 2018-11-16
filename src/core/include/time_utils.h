#pragma once
#include <ctime>
#include <string>

using namespace std;

namespace time_utils
{
	// Create a time_t date with the given information
	time_t create_date(int day, int month, int year, int hours = 0,
		               int minutes = 0, int seconds = 0);

	// Convert a time_t to a rfc3339 string
	string to_rfc3339(time_t t);

	// Convert a rfc3339 string to a time_t
	time_t from_rfc3339(string s);
}