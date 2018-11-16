#include "time_utils.h"
#include <cstdio>

time_t time_utils::create_date(int day, int month, int year, int hours,
		                       int minutes, int seconds)
{
	struct tm timeinfo = {
		seconds,
		minutes,
		hours,
		day,
		month - 1,
		year - 1900,
		0, // osef
		0, // osef
		-1
	};
	return mktime(&timeinfo);
}

string time_utils::to_rfc3339(time_t t)
{
	char buffer[30];
	struct tm * timeinfo = localtime(&t);

	strftime(buffer, 30, "%Y-%m-%dT%H:%M:%SZ", timeinfo);

	return string(buffer);
}


time_t time_utils::from_rfc3339(string s)
{
	int year, month, day, hours, minutes, seconds;
	sscanf(s.c_str(), "%d-%d-%dT%d:%d:%dZ",
		   &year, &month, &day, &hours, &minutes, &seconds);

	return create_date(day, month, year, hours, minutes, seconds);
}