#pragma once
#include <iostream>
#include <sstream>
#include "../Machine/Machine.h"

class Reader
{
public:
    Machine static ReadMealyMachine(std::istream& istream);
    Machine static ReadMooreMachine(std::istream& istream);

private:
    void static ParseStr(const std::string& str, std::vector<std::string>& items);
    void static ProcessStr(std::istream& istream, std::vector<std::string>& machineVector);
    void static ProcessTable(std::istream& istream, Machine& machine);
};