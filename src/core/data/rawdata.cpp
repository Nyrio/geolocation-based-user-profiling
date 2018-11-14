#include "rawdata.h"
#include <iostream>

using namespace std;

map<int, data::PointSet> data::rawdata_query(int id) {
	// TODO change this to a true call to DB
	// where ID = id
	cout << "The raw data says hi" << endl;
	map<int, PointSet> u;
	return u;
}
map<int, data::PointSet> data::rawdata_query(int id, time_t t1, time_t t2) {
	// TODO change this to a true call to DB
	// where ID = id && t1 <= TIMESTAMP <= t2
	cout << "The raw data says hello" << endl;
	map<int, PointSet> u;
	return u;
}

