#pragma once
#include <vector>
#include <string>

enum class MachineType
{
    Mealy,
    Moore
};

struct Machine
{
    std::vector<std::string> inputData;
    std::vector<std::string> states;
    std::vector<std::string> outputData;
    std::vector < std::vector<std::string>> transitions;
};
