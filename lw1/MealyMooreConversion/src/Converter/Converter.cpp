#include "Converter.h"

Machine Converter::GetMooreMachineFromMealy(const Machine& mealyMachine)
{
	Machine mooreMachine = {};

	mooreMachine.inputData = mealyMachine.inputData;

	std::map<std::string, std::string> transitionsMap = GetUniqueTransitions(mealyMachine);
	GetMooreTransitions(transitionsMap, mealyMachine, mooreMachine);

	return mooreMachine;
}

Machine Converter::GetMealyMachineFromMoore(const Machine& mooreMachine)
{
	Machine mealyMachine = {};

	mealyMachine.inputData = mooreMachine.inputData;
	mealyMachine.states = mooreMachine.states;

	GetMealyTransitions(mooreMachine, mealyMachine);

	return mealyMachine;
}

bool Converter::IsValueExist(const std::string& transition, const std::map<std::string, std::string>& transitionsMap)
{
	for (const auto& pair : transitionsMap)
	{
		if (pair.second == transition)
		{
			return true;
		}
	}
	return false;
}

std::pair<std::string, std::string> Converter::ParseMealyTransition(const std::string& transition)
{
	std::string state;
	std::string outputData;
	std::istringstream transitionStream(transition);

	std::getline(transitionStream, state, '/');
	std::getline(transitionStream, outputData, '/');

	return std::pair<std::string, std::string>(state, outputData);
}

std::string Converter::GetMooreTransition(const std::string& transition, const std::map<std::string, std::string>& transitionsMap)
{
	for (const auto& pair : transitionsMap)
	{
		if (pair.second == transition)
		{
			return pair.first;
		}
	}
}

std::map<std::string, std::string> Converter::GetUniqueTransitions(const Machine& mealyMachine)
{
	std::map<std::string, std::string> transitionsMap;
	for (auto& transitionVector : mealyMachine.transitions)
	{
		for (auto& transition : transitionVector)
		{
			if (!IsValueExist(transition, transitionsMap))
			{
				transitionsMap.insert(std::pair<std::string, std::string>(NEW_STATE + std::to_string(transitionsMap.size()), transition));
			}
		}
	}
	return transitionsMap;
}

void Converter::GetMooreTransitions(const std::map<std::string, std::string>& transitionsMap, const Machine& mealyMachine, Machine& mooreMachine)
{
	for (const auto& pair : transitionsMap)
	{
		mooreMachine.states.push_back(pair.first);
		mooreMachine.outputData.push_back(ParseMealyTransition(pair.second).second);

		std::string state = ParseMealyTransition(pair.second).first;
		auto it = std::find(mealyMachine.states.begin(), mealyMachine.states.end(), state);
		auto pos = std::to_string(std::distance(mealyMachine.states.begin(), it));
		std::vector<std::string> unitVector;
		for (auto i = 0; i < mealyMachine.inputData.size(); i++)
		{
			unitVector.push_back(GetMooreTransition(mealyMachine.transitions[std::stoi(pos)][i], transitionsMap));
		}

		mooreMachine.transitions.push_back(unitVector);
	}
}

void Converter::GetMealyTransitions(const Machine& mooreMachine, Machine& mealyMachine)
{
	for (auto& transitionVector : mooreMachine.transitions)
	{
		std::vector<std::string> unitVector;
		for (auto& transition : transitionVector)
		{
			auto it = std::find(mooreMachine.states.begin(), mooreMachine.states.end(), transition);
			auto pos = std::to_string(std::distance(mooreMachine.states.begin(), it));
			std::string outputData = mooreMachine.outputData[std::stoi(pos)];
			unitVector.push_back(transition + '/' + outputData);
		}
		mealyMachine.transitions.push_back(unitVector);
	}
}