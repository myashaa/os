#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include "rule.h"

using namespace std;

class CHandler
{
public:
    void static InitializeArr(std::ifstream& table, std::vector<Rule>& arr);
    bool static CompareLine(std::vector<Rule> const& arr, std::string const& line);

private:
    string static ReadStringPart(string& str, char separator);
    void static RemoveExtraSpaces(string& str);
};