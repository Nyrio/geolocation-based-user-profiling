#pragma once

#include <string>

namespace data{
    struct WorkflowParam {
        double eps;
        int minPts;

        float buildingRadius;
    };
    WorkflowParam loadParam(std::string file = "./config.txt");
    void parseWorkflow(WorkflowParam &w, std::string s);
    void resetWorkflow(WorkflowParam &w);
}