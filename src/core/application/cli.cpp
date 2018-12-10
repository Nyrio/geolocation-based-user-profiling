#include <iostream>
#include <vector>
#include <string.h>
#include "Core.h"
#include "time_utils.h"
#include "string_utils.h"

using namespace std;

void execute(services::Core &c, const string &s);
void print_help();

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

void execute(services::Core &c, const string &s)
{
	vector<string> words = split(s, ' ');
	if (words.size() > 0)
	{

		if (words[0].compare("load") == 0 && words.size() >= 2)
		{
			uint id;
			time_t t1, t2;

			try
			{
				id = stoi(words[1]);
				if(words.size() >= 4) {
					t1 = time_utils::from_rfc3339(words[2]);
					t2 = time_utils::from_rfc3339(words[3]);
				}
				else {
					t1 = 0;
					t2 = 0;
				}
			}
			catch(...) {
				cout << "Invalid parameters for load <uid> [<t1> <t2>]" << endl;
			}
			c.clusterize(id, t1, t2);
		}
		else if(words[0].compare("house") == 0)
		{
			c.print_house();
		}
		else if(words[0].compare("workplace") == 0)
		{
				c.print_work();
		}
		else if (words[0].compare("show-clusters") == 0)
		{
			c.show_clusters();
		}
		else if (words[0].compare("frequent-places") == 0)
		{
			c.analyze_tags();
		}
		else if (words[0].compare("help") == 0)
		{
			print_help();
		}
		/*
		* Testing commands
		*/
		else if (words[0].compare("test") == 0 && words.size() >= 2)
		{
			if(words[1] == "clustering")
				c.testDJClustering();
			else if(words[1] == "cluster-features")
				c.test_cluster_features();
			else if(words[1] == "visits")
				c.test_clusters_visits();
		}
		else if (words[0].compare("benchmark") == 0 && words.size() >= 2)
		{
			if(words[1] == "clustering" && words.size() >= 7)
			{
				uint id = stoi(words[2]);
				time_t t1 = time_utils::from_rfc3339(words[3]);
				time_t t2 = time_utils::from_rfc3339(words[4]);
				uint nbmax = stoi(words[5]);
				uint nbmes = stoi(words[6]);
				c.benchmark_clustering(id, t1, t2, nbmax, nbmes);
			}
		}
		else if (words[0].compare("XXXXXXX") == 0)
		{
			// TODO
		}
		/*
		* End of testing commands
		*/ 
		else
		{
			cout << "Unknown command" << endl;
		}
	}
}

void print_help()
{
	cout << endl << "First you have to specify the user (and optionnaly the time range) to work on : " << endl
	<< "    >>> load id [t1 t2]" << endl
	<< "Then you can find information :" << endl
	<< "Find where the user lives. This gives 2 main living-points (and another ignoring durations)" << endl
	<< "    >>> house" << endl
	<< "Find where the user works. This give a place (and another ignoring durations)" << endl
	<< "    >>> workplace" << endl
	<< "Find tags about the places " << endl
	<< "    >>> frequent-places" << endl
	<< "Show all clusters found" << endl
	<< "    >>> show-clusters" << endl << endl
	<< "To leave the application :" << endl
	<< "    >>> exit" << endl << endl;
}
