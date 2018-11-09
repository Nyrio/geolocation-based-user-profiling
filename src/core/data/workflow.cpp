#include <iostream>
#include <fstream>

#include "workflow.h"

data::WorkflowParam data::loadParam(std::string file)
{
    data::WorkflowParam w;
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

    w.param1 = file;
    return w;
}

void data::parseWorkflow(WorkflowParam w, std::string s){
    std::cout<<" We read : "<<s<<std::endl;
}