#include <iostream>
#include <vector>
#include <sstream>
#include <string.h>
#include "Core.h"

using namespace std;

void execute(services::Core &c, const string &s);

int main(int argc, char *argv[])
{

	// default main arguments
	string wp = "";
	// main arguments parsing
	for (int ite = 1; ite < argc; ++ite)
	{
		if (strcmp("-wp", argv[ite]) == 0)
		{
			if (ite + 1 < argc)
			{
				wp = argv[++ite];
			}
			else
			{
				cerr << "-wp parameter must be followed by a string" << endl;
			}
		}
	}
	// end of argument parsings

	// set up
	services::Core c(wp);

	// wait for user inputs.
	bool finished = false;
	string s;
	while (!finished)
	{
		cout << ">>> ";
		getline(cin, s);
		if (s.compare("exit") == 0)
			finished = true;
		else 
			execute(c, s);
	}

	return 0;
}

vector<string> split(const string &s, char delim)
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

void execute(services::Core &c, const string &s)
{
	vector<string> words = split(s, ' ');
	if (words.size() > 0)
	{
		if (words[0].compare("test") == 0)
		{
			// Temp tests, to remove :
			c.test_cluster_features();
		}
		else if (words[0].compare("XXXXXXX") == 0)
		{
			// TODO
		}
	}
}
