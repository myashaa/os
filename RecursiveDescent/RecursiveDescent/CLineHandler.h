#pragma once
#include <fstream>
#include <algorithm>
#include <stack>
#include "exceptions.h"

using namespace std;

class CLineHandler
{
public:
    bool static ReadLine(ifstream& input, string& str);
    void static RemoveExtraSpaces(string& str);
    string static GetSubstring(string const& str, string const& separartor);

    bool static IsCorrectBrackets(string const& str, string& errorMessage);
    void static RemoveExtraBrackets(string& str);
    bool static IsOperationHaveOpenBracket(string const& str, string const& operation);

private:
    void static ToLower(string& str);
};