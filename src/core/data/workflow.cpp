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
    w.param1 = "default param1";
    w.param2 = 0;
    w.eps = 0.0002;
    w.minPts = 20;
}

void data::parseWorkflow(WorkflowParam &w, std::string s)
{
    std::string delim = "=";
    std::string name = s.substr(0, s.find(delim));
    std::string value = s.substr(s.find(delim) + delim.length(), s.length());

    //example params to extends to new parameters
    if(name.compare("param1") == 0){
        w.param1 = value;
    } else if(name.compare("paramNum") == 0) {
        w.param2 = std::stoi(value);
    } else if (name.compare("eps") == 0) {
        w.eps = std::stod(value);
    } else if (name.compare("minPts") == 0) {
        w.minPts = std::stoi(value);
    }
}