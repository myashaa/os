#pragma once
#include <fstream>

using namespace std;

class CParser
{
public:
    bool static Parse(ifstream& input, string& errorMessage);

private:
    bool static ParseProg(ifstream& input, string& errorMessage, string& str);
    bool static ParseVar(ifstream& input, string& errorMessage, string& str);
    bool static ParseBegin(ifstream& input, string& errorMessage, string& str);
    bool static ParseEnd(string& errorMessage, string& str);

    bool static VarParser(string& str, string& errorMessage);
    bool static IDListParser(string& str, string& errorMessage);
    bool static TypeParser(string& str, string& errorMessage);
    bool static ListSTParser(ifstream& input, string& str, string& errorMessage);
    bool static STParser(string const& str, string& errorMessage);
    bool static ReadParser(string& str, string& errorMessage);
    bool static WriteParser(string& str, string& errorMessage);
    bool static AssignParser(string& str, string& errorMessage);
    bool static EXPParser(string& str, string& errorMessage);
    bool static TParser(string& str, string& errorMessage);
    bool static FParser(string& str, string& errorMessage);

    bool static OperationParser(string const& operation, string& str, string& errorMessage);
};