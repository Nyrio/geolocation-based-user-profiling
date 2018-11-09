#include "rawdata.h"
#include <iostream>

using namespace std;

void data::rawdata_query(int id) {
	cout << "The raw data says hi" << endl;
	UserData u;
	int size = u.list.size();
	cout << to_string(size)<<endl;
}
