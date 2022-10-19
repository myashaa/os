#include "Reader.h"

Machine Reader::ReadMealyMachine(std::istream& istream)
{
    Machine machine = {};

    ProcessStr(istream, machine.states);
    ProcessTable(istream, machine);

    return machine;
}

Machine Reader::ReadMooreMachine(std::istream& istream)
{
    Machine machine = {};

    ProcessStr(istream, machine.outputData);
    ProcessStr(istream, machine.states);
    ProcessTable(istream, machine);

    return machine;
}

void Reader::ParseStr(const std::string& str, std::vector<std::string>& items)
{
    std::string item;
    std::istringstream strStream(str);
    while (std::getline(strStream, item, ';'))
    {
        items.push_back(item);
    }
}

void Reader::ProcessStr(std::istream& istream, std::vector<std::string>& machineVector)
{
    std::string str;
    std::getline(istream, str);
    std::vector<std::string> items;
    ParseStr(str, items);
    items.erase(items.begin());
    machineVector = items;
}

void Reader::ProcessTable(std::istream& istream, Machine& machine)
{
    std::string str;
    while (std::getline(istream, str))
    {
        std::vector<std::string> values;
        ParseStr(str, values);
        machine.inputData.push_back(values.front());
        values.erase(values.begin());

        int i = 0;
        for (auto& value : values)
        {
            if (machine.transitions.size() != machine.states.size())
            {
                std::vector<std::string> unitVector;
                unitVector.push_back(value);
                machine.transitions.push_back(unitVector);
            }
            else
            {
                machine.transitions[i].push_back(value);
                i++;
            }
        }
    }
}