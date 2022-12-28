#include "CParser.h"

using namespace std;

int main(int argc, char* argv[])
{
	ifstream input(argv[1]);

	ofstream out("output.txt");

	string errorMessage;

	if (CParser::Parse(input, errorMessage))
	{
		out << "Ok";
	}
	else
	{
		out << errorMessage << endl;
	}

	return 0;
}