#pragma once
#include <algorithm> 
#include <sstream>
#include "../Machine/Machine.h"

const char FIRST_CLASS_NAME = 'A';
typedef std::vector<std::vector<std::string>> TableNxN;
typedef std::vector<std::pair<std::string, std::vector<std::string>>> TableNx2;

class Minimizer
{
public:
    Machine static GetMinimizedMealyMachine(const Machine& machine);
    Machine static GetMinimizedMooreMachine(const Machine& machine);

private:
    std::pair<std::string, std::string> static ParseMealyTransition(const std::string& transition);
    void static CreateUniqueValues(const Machine& machine, TableNx2& uniqueValues, TableNx2& statesInClass, TableNxN& table);
    void static CreateUniqueValuesAgain(const Machine& machine, TableNx2& uniqueValues, TableNx2& statesInClass, TableNxN& table, TableNxN& newTable);
    bool static IsMinimized(const TableNx2& uniqueValues, const TableNx2& newUniqueValues);
    void static CreateTable(const Machine& machine, const TableNx2& statesInClass, TableNxN& table, int index);
    void static CreateMinimizedTable(const Machine& machine, TableNx2& uniqueValues, TableNx2& statesInClass, TableNxN& table);
    void static CreateColumnForMinimizedMealyMachine(const Machine& machine, const TableNxN& table, Machine& minimizedMachine, int index);
    void static CreateColumnForMinimizedMooreMachine(const Machine& machine, const TableNxN& table, Machine& minimizedMachine, int index);
    Machine static GetMinimizedMachine(const Machine& machine, const TableNxN& table);
};