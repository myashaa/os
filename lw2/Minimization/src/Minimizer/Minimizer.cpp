#include "Minimizer.h"

int GetIndex(const std::string& value)
{
	std::string str = value;
	str.erase(str.begin());
	return std::stoi(str);
}

bool cmpToSortByClassAndState(std::vector<std::string>& a, std::vector<std::string>& b)
{
	int firstClassIndex = GetIndex(a[0]);
	int secondClassIndex = GetIndex(b[0]);
	int firstStateIndex = GetIndex(a[1]);
	int secondStateIndex = GetIndex(b[1]);

	return (firstClassIndex < secondClassIndex) || ((firstClassIndex == secondClassIndex) && (firstStateIndex < secondStateIndex));
}

bool cmpToSortByState(std::vector<std::string>& a, std::vector<std::string>& b)
{
	int firstStateIndex = GetIndex(a[1]);
	int secondStateIndex = GetIndex(b[1]);

	return firstStateIndex < secondStateIndex;
}


Machine Minimizer::GetMinimizedMealyMachine(const Machine& machine)
{
	TableNxN table;
	table.push_back({ FIRST_CLASS_NAME + std::to_string(1), machine.states[0] });

	std::vector<std::string> vec;
	for (const auto& transition : machine.transitions[0])
	{
		vec.push_back(ParseMealyTransition(transition).second);
	}

	TableNx2 uniqueValues = { { FIRST_CLASS_NAME + std::to_string(1) , vec } };
	TableNx2 statesInClass = { { FIRST_CLASS_NAME + std::to_string(1) , { machine.states[0] } } };

	CreateMinimizedTable(machine, uniqueValues, statesInClass, table);

	Machine minimizedMachine = GetMinimizedMachine(machine, table);

	return minimizedMachine;
}

Machine Minimizer::GetMinimizedMooreMachine(const Machine& machine)
{
	TableNxN table;
	table.push_back({ FIRST_CLASS_NAME + std::to_string(1), machine.states[0] });

	TableNx2 uniqueValues = { { FIRST_CLASS_NAME + std::to_string(1) , { machine.outputData[0] } } };
	TableNx2 statesInClass = { { FIRST_CLASS_NAME + std::to_string(1) , { machine.states[0] } } };

	CreateMinimizedTable(machine, uniqueValues, statesInClass, table);

	Machine minimizedMachine = GetMinimizedMachine(machine, table);

	return minimizedMachine;
}


std::pair<std::string, std::string> Minimizer::ParseMealyTransition(const std::string& transition)
{
	std::string state;
	std::string outputData;
	std::istringstream transitionStream(transition);

	std::getline(transitionStream, state, '/');
	std::getline(transitionStream, outputData, '/');

	return { state, outputData };
}

void Minimizer::CreateUniqueValues(const Machine& machine, TableNx2& uniqueValues, TableNx2& statesInClass, TableNxN& table)
{
	int count = 1;

	for (auto i = 1; i < machine.transitions.size(); i++)
	{
		std::vector<std::string> vec;
		std::string str;
		if (machine.type == MachineType::Mealy)
		{
			for (const auto& transition : machine.transitions[i])
			{
				vec.push_back(ParseMealyTransition(transition).second);
			}
		}
		else
		{
			str = machine.outputData[i];
		}

		bool isNewClass = true;
		for (const auto& unique : uniqueValues)
		{
			if (((machine.type == MachineType::Mealy) && (vec == unique.second)) || ((machine.type == MachineType::Moore) && (str == unique.second[0])))
			{
				isNewClass = false;
				statesInClass.push_back({ unique.first, { machine.states[i] } });
				table.push_back({ unique.first, machine.states[i] });
				break;
			}
		}
		if (isNewClass)
		{
			count++;
			statesInClass.push_back({ FIRST_CLASS_NAME + std::to_string(count), { machine.states[i] } });
			table.push_back({ FIRST_CLASS_NAME + std::to_string(count), machine.states[i] });
			if (machine.type == MachineType::Mealy)
			{
				uniqueValues.push_back({ FIRST_CLASS_NAME + std::to_string(count), vec });
			}
			else
			{
				uniqueValues.push_back({ FIRST_CLASS_NAME + std::to_string(count), { str } });
			}
		}
	}
}

void Minimizer::CreateUniqueValuesAgain(const Machine& machine, TableNx2& uniqueValues, TableNx2& statesInClass, TableNxN& table, TableNxN& newTable)
{
	int count = 1;

	std::vector<std::string> columnInTableWithoutClass = table[0];
	columnInTableWithoutClass.erase(columnInTableWithoutClass.begin() + 1);

	char letter = table[0][0][0];
	letter = (char)(letter + 1);

	for (auto i = 1; i < machine.transitions.size(); i++)
	{
		bool isNewClass = true;
		for (const auto& unique : uniqueValues)
		{
			columnInTableWithoutClass = table[i];
			columnInTableWithoutClass.erase(columnInTableWithoutClass.begin() + 1);
			if (columnInTableWithoutClass == unique.second)
			{
				isNewClass = false;
				statesInClass.push_back({ unique.first, table[i] });
				newTable.push_back({ unique.first, table[i][1] });
				table[i].push_back(unique.first);
				break;
			}
		}
		if (isNewClass)
		{
			count++;
			statesInClass.push_back({ letter + std::to_string(count), table[i] });
			uniqueValues.push_back({ letter + std::to_string(count), columnInTableWithoutClass });
			newTable.push_back({ letter + std::to_string(count), table[i][1] });
			table[i].push_back(letter + std::to_string(count));
		}
	}
}

bool Minimizer::IsMinimized(const TableNx2& uniqueValues, const TableNx2& newUniqueValues)
{
	if (uniqueValues.size() == newUniqueValues.size())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Minimizer::CreateTable(const Machine& machine, const TableNx2& statesInClass, TableNxN& table, int index = 1)
{
	for (auto i = 0; i < machine.transitions.size(); i++)
	{
		for (const auto& transition : machine.transitions[i])
		{
			for (const auto& state : statesInClass) {
				std::string transitionStr = transition;
				if (machine.type == MachineType::Mealy)
				{
					transitionStr = ParseMealyTransition(transitionStr).first;
				}

				if (transitionStr == state.second[index])
				{
					table[i].push_back(state.first);
					break;
				}
			}
		}
	}
}

void Minimizer::CreateMinimizedTable(const Machine& machine, TableNx2& uniqueValues, TableNx2& statesInClass, TableNxN& table)
{
	char letter = FIRST_CLASS_NAME;

	CreateUniqueValues(machine, uniqueValues, statesInClass, table);
	CreateTable(machine, statesInClass, table, 0);
	sort(table.begin(), table.end(), cmpToSortByClassAndState);

	while (true)
	{
		letter = (char)(letter + 1);

		TableNxN newTable;
		newTable.push_back({ letter + std::to_string(1), machine.states[0] });

		std::vector<std::string> columnInTableWithoutClass = table[0];
		columnInTableWithoutClass.erase(columnInTableWithoutClass.begin() + 1);

		TableNx2 newUniqueValues = { { letter + std::to_string(1) , columnInTableWithoutClass } };
		TableNx2 newStatesInClass = { { letter + std::to_string(1) , table[0] } };
		table[0].push_back(letter + std::to_string(1));

		CreateUniqueValuesAgain(machine, newUniqueValues, newStatesInClass, table, newTable);

		if (IsMinimized(uniqueValues, newUniqueValues))
			break;

		sort(newTable.begin(), newTable.end(), cmpToSortByState);
		CreateTable(machine, newStatesInClass, newTable);
		sort(newTable.begin(), newTable.end(), cmpToSortByClassAndState);

		table = newTable;
		uniqueValues = newUniqueValues;
		statesInClass = newStatesInClass;
	}
}

void Minimizer::CreateColumnForMinimizedMealyMachine(const Machine& machine, const TableNxN& table, Machine& minimizedMachine, int index)
{
	std::vector<std::string> vec = table[index];
	vec.erase(vec.begin(), vec.begin() + 2);
	vec.pop_back();
	minimizedMachine.transitions.push_back(vec);

	auto it = std::find(machine.states.begin(), machine.states.end(), table[index][1]);
	auto pos = std::to_string(std::distance(machine.states.begin(), it));
	for (auto i = 0; i < minimizedMachine.transitions[0].size(); i++)
	{
		int ind = minimizedMachine.transitions.size() - 1;
		minimizedMachine.transitions[ind][i] = minimizedMachine.transitions[ind][i] + '/' + ParseMealyTransition(machine.transitions[std::stoi(pos)][i]).second;
	}
}

void Minimizer::CreateColumnForMinimizedMooreMachine(const Machine& machine, const TableNxN& table, Machine& minimizedMachine, int index)
{
	auto it = std::find(machine.states.begin(), machine.states.end(), table[index][1]);
	auto pos = std::to_string(std::distance(machine.states.begin(), it));

	minimizedMachine.outputData.push_back(machine.outputData[std::stoi(pos)]);

	std::vector<std::string> vec = table[index];
	vec.erase(vec.begin(), vec.begin() + 2);
	vec.pop_back();
	minimizedMachine.transitions.push_back(vec);
}

Machine Minimizer::GetMinimizedMachine(const Machine& machine, const TableNxN& table)
{
	Machine minimizedMachine = {};

	minimizedMachine.inputData = machine.inputData;
	minimizedMachine.states.push_back(table[0][0]);

	if (machine.type == MachineType::Mealy)
	{
		CreateColumnForMinimizedMealyMachine(machine, table, minimizedMachine, 0);
	}
	else
	{
		CreateColumnForMinimizedMooreMachine(machine, table, minimizedMachine, 0);
	}

	for (auto i = 1; i < table.size(); i++)
	{
		if (table[i - 1][0] != table[i][0])
		{
			if (machine.type == MachineType::Mealy)
			{
				CreateColumnForMinimizedMealyMachine(machine, table, minimizedMachine, i);
			}
			else
			{
				CreateColumnForMinimizedMooreMachine(machine, table, minimizedMachine, i);
			}

			minimizedMachine.states.push_back(table[i][0]);
		}
	}

	return minimizedMachine;
}