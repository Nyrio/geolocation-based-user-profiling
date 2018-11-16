#include "string_utils.h"
#include <sstream>

using namespace std;

vector<string> split(const string &s, const char delim)
{
	stringstream ss(s);
	string item;
	vector<string> elems;
	while (getline(ss, item, delim))
	{
		elems.push_back(item);
	}
	return elems;
}

string str_join(const vector<string>& vs, const string delim)
{
	stringstream ss;
	for(size_t i = 0; i < vs.size(); ++i)
	{
	  if(i != 0)
	    ss << delim;
	  ss << vs[i];
	}
	string s = ss.str();
	return s;
}