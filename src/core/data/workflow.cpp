#include <iostream>
#include <fstream>

#include "workflow.h"

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

    w.buildingRadius = 5.;
}

void data::parseWorkflow(WorkflowParam &w, std::string s)
{
    if(s.length() > 1 && s[0] != '#')
    {
        std::string delim = "=";
        std::string name = s.substr(0, s.find(delim));
        std::string value = s.substr(s.find(delim) + delim.length(), s.length());

        // different parameters expected
        if (name.compare("eps") == 0) {
            w.eps = std::stod(value);
        } else if (name.compare("min_pts") == 0) {
            w.minPts = std::stoi(value);
        } else if (name.compare("building_range") == 0) {
            w.buildingRadius = std::stof(value);
        }
    }
}