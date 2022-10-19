#pragma once
#include <ostream>
#include "../Machine/Machine.h"

class Writer
{
public:
    void static WriteMooreMachine(const Machine& machine, std::ostream& ostream);
    void static WriteMealyMachine(const Machine& machine, std::ostream& ostream);

private:
    void static PrintStr(std::ostream& ostream, const std::vector<std::string>& machineVector);
    void static PrintTable(std::ostream& ostream, const Machine& machine);
};