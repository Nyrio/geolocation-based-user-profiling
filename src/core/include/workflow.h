#pragma once

#include <string>

namespace data{
    struct WorkflowParam {
        std::string param1;
        int param2;
    };
    WorkflowParam loadParam(std::string file = "./config.txt");
    void parseWorkflow(WorkflowParam &w, std::string s);
    void resetWorkflow(WorkflowParam &w);
}