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
    void static InitializeArr(vector<Rule>& arr);
    bool static CompareLine(vector<Rule> const& arr, string line);
};