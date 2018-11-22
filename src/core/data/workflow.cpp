#include "workflow.h"
#include <iostream>
#include <fstream>
#include "string_utils.h"

using namespace std;

data::WorkflowParam data::loadParam(string file)
{
    data::WorkflowParam w;
    data::resetWorkflow(w);

    string line;
    ifstream myfile(file);
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            data::parseWorkflow(w, line);
        }
        myfile.close();
    }
    else
    {
        cerr << "Unable to open file" << endl;
    }


    return w;
}

void data::resetWorkflow(WorkflowParam &w){
    w.eps = 0.0002f;
    w.minPts = 20;

    w.searchRadius = 10.;
    w.sepMinutes = 10.;
    w.placeTypes = vector<string>();
}

void data::parseWorkflow(WorkflowParam &w, string s)
{
    if(s.length() > 1 && s[0] != '#')
    {
        string delim = "=";
        string name = s.substr(0, s.find(delim));
        string value = s.substr(s.find(delim) + delim.length(), s.length());

        // different parameters expected
        if (name == "eps") {
            w.eps = stof(value);
        } else if (name == "min_pts") {
            w.minPts = stoi(value);
        } else if (name == "search_radius") {
            w.searchRadius = stof(value);
        } else if (name == "sep_minutes") {
            w.sepMinutes = stof(value);
        } else if (name == "place_types") {
            w.placeTypes = split(value, ',');
        }
    }
}