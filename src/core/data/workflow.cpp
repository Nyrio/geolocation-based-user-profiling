#include "workflow.h"
#include <iostream>
#include <fstream>
#include "string_utils.h"

using namespace std;

data::WorkflowParam data::loadParam(std::string file)
{
    data::WorkflowParam w;
    data::resetWorkflow(w);

    std::string line;
    std::ifstream myfile(file);
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
        std::cerr << "Unable to open file" << std::endl;
    }


    return w;
}

void data::resetWorkflow(WorkflowParam &w){
    w.eps = 0.0002;
    w.minPts = 20;

    w.searchRadius = 10.;
    w.sepMinutes = 10.;
    w.placeTypes = vector<string>();
}

void data::parseWorkflow(WorkflowParam &w, std::string s)
{
    if(s.length() > 1 && s[0] != '#')
    {
        std::string delim = "=";
        std::string name = s.substr(0, s.find(delim));
        std::string value = s.substr(s.find(delim) + delim.length(), s.length());

        // different parameters expected
        if (name == "eps") {
            w.eps = std::stod(value);
        } else if (name == "min_pts") {
            w.minPts = std::stoi(value);
        } else if (name == "search_radius") {
            w.searchRadius = std::stof(value);
        } else if (name == "sep_minutes") {
            w.sepMinutes = std::stof(value);
        } else if (name == "place_types") {
            w.placeTypes = split(value, ',');
        }else if (name == "precision_sec") {
            w.precisionSec = std::stof(value);
        }
    }
}