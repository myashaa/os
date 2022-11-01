#include "Writer.h"

void Writer::WriteMooreMachine(const Machine& machine, std::ostream& ostream)
{
	PrintStr(ostream, machine.outputData);
	PrintStr(ostream, machine.states);
	PrintTable(ostream, machine);
}

void Writer::WriteMealyMachine(const Machine& machine, std::ostream& ostream)
{
	PrintStr(ostream, machine.states);
	PrintTable(ostream, machine);
}

void Writer::PrintStr(std::ostream& ostream, const std::vector<std::string>& machineVector)
{
	for (auto& item : machineVector)
	{
		ostream << ";" << item;
	}
	ostream << std::endl;
}

void Writer::PrintTable(std::ostream& ostream, const Machine& machine)
{
	auto size = machine.states.size();
	for (auto i = 0; i < machine.inputData.size(); i++)
	{
		ostream << machine.inputData[i];
		for (auto j = 0; j < size; j++)
		{
			ostream << ";" << machine.transitions[j][i];
		}
		ostream << std::endl;
	}
}