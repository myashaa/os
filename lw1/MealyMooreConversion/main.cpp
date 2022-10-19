#include <iostream>
#include <fstream>
#include "src/Machine/Machine.h"
#include "src/Reader/Reader.h"
#include "src/Converter/Converter.h"
#include "src/Writer/Writer.h"

const std::string MEALY_MOORE_MODE = "mealy-to-moore";
const std::string MOORE_MEALY_MODE = "moore-to-mealy";

struct Args
{
    MachineType machineType;
    std::string fileIn;
    std::string fileOut;
};

bool ParseArgs(int argc, char* argv[], Args& args);
bool InitFilesStreams(Args& args, std::ifstream& fileIn, std::ofstream& fileOut);

int main(int argc, char* argv[])
{
    Args args;
    bool isCorrect = ParseArgs(argc, argv, args);
    if (!isCorrect)
    {
        std::cout << "Invalid input data!" << std::endl;
        std::cout << "Enter in the format: .exe mealy-to-moore <input file> <output file>" << std::endl;
        return 1;
    }

    std::ifstream fileIn;
    std::ofstream fileOut;
    isCorrect = InitFilesStreams(args, fileIn, fileOut);
    if (!isCorrect)
    {
        return 1;
    }

    Machine mealyMachine, mooreMachine;
    if (args.machineType == MachineType::Mealy)
    {
        mealyMachine = Reader::ReadMealyMachine(fileIn);
        mooreMachine = Converter::GetMooreMachineFromMealy(mealyMachine);
        Writer::WriteMooreMachine(mooreMachine, fileOut);
    }
    else
    {
        mooreMachine = Reader::ReadMooreMachine(fileIn);
        mealyMachine = Converter::GetMealyMachineFromMoore(mooreMachine);
        Writer::WriteMealyMachine(mealyMachine, fileOut);
    }

    return 0;
}

bool ParseArgs(int argc, char* argv[], Args& args)
{
    if (argc != 4)
    {
        return false;
    }

    if (argv[1] == MEALY_MOORE_MODE)
    {
        args.machineType = MachineType::Mealy;
    }
    else if (argv[1] == MOORE_MEALY_MODE)
    {
        args.machineType = MachineType::Moore;
    }
    else
    {
        return false;
    }

    args.fileIn = argv[2];
    args.fileOut = argv[3];

    return true;
}

bool InitFilesStreams(Args& args, std::ifstream& fileIn, std::ofstream& fileOut)
{
    fileIn.open(args.fileIn);
    if (!fileIn.is_open())
    {
        std::cout << "Error opening input file!" << std::endl;
        return false;
    }

    fileOut.open(args.fileOut);
    if (!fileOut.is_open())
    {
        std::cout << "Error opening output file!" << std::endl;
        return false;
    }

    return true;
}