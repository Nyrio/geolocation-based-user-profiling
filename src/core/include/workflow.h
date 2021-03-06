#pragma once

#include <string>
#include <vector>

namespace data{
    struct WorkflowParam {
        float eps;
        int minPts;

        float searchRadius;
        float sepMinutes;
        std::vector<std::string> placeTypes;

        int startNight;
        int endNight;
        int startWork;
        int endWork;

        float precisionSec;
    };
    WorkflowParam loadParam(std::string file = "./config.txt");
    void parseWorkflow(WorkflowParam &w, std::string s);
    void resetWorkflow(WorkflowParam &w);
}