#pragma once

#include <string>
#include <vector>

using namespace std;

namespace data{
    struct WorkflowParam {
        float eps;
        int minPts;

        float searchRadius;
        float sepMinutes;
        vector<string> placeTypes;
    };
    WorkflowParam loadParam(std::string file = "./config.txt");
    void parseWorkflow(WorkflowParam &w, std::string s);
    void resetWorkflow(WorkflowParam &w);
}