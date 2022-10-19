#pragma once
#include <map>
#include <sstream>
#include "../Machine/Machine.h"

const std::string NEW_STATE = "q";

class Converter
{
public:
    Machine static GetMooreMachineFromMealy(const Machine& machine);
    Machine static GetMealyMachineFromMoore(const Machine& machine);

private:
    bool static IsValueExist(const std::string& transition, const std::map<std::string, std::string>& transitionsMap);
    std::pair<std::string, std::string> static ParseMealyTransition(const std::string& transition);
    std::string static GetMooreTransition(const std::string& transition, const std::map<std::string, std::string>& transitionsMap);
    std::map<std::string, std::string> static GetUniqueTransitions(const Machine& mealyMachine);
    void static GetMooreTransitions(const std::map<std::string, std::string>& transitionsMap, const Machine& mealyMachine, Machine& mooreMachine);
    void static GetMealyTransitions(const Machine& mooreMachine, Machine& mealyMachine);
};