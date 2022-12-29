#include "CHandler.h"

int main(int argc, char* argv[])
{
    ifstream input(argv[1]);

    ofstream out("output.txt");

    vector<Rule> arr;
    CHandler::InitializeArr(arr);

    string line;
    while (getline(input, line))
    {
        out << line << ":  is ";
        if (CHandler::CompareLine(arr, line))
        {
            out << "Ok\n";
        }
        else
        {
            out << "Error\n";
        }
    }

    return 0;
}
