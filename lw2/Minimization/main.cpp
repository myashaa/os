#include <iostream>
#include <fstream>
#include "src/Machine/Machine.h"
#include "src/Reader/Reader.h"
#include "src/Minimizer/Minimizer.h"
#include "src/Writer/Writer.h"

const std::string MEALY_MODE = "mealy";
const std::string MOORE_MODE = "moore";

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

    Machine machine, minimizedMachine;
    if (args.machineType == MachineType::Mealy)
    {
        machine = Reader::ReadMealyMachine(fileIn);
        machine.type = args.machineType;
        minimizedMachine = Minimizer::GetMinimizedMealyMachine(machine);
        Writer::WriteMealyMachine(minimizedMachine, fileOut);
    }
    else
    {
        machine = Reader::ReadMooreMachine(fileIn);
        machine.type = args.machineType;
        minimizedMachine = Minimizer::GetMinimizedMooreMachine(machine);
        Writer::WriteMooreMachine(minimizedMachine, fileOut);
    }

    return 0;
}

bool ParseArgs(int argc, char* argv[], Args& args)
{
    if (argc != 4)
    {
        return false;
    }

    if (argv[1] == MEALY_MODE)
    {
        args.machineType = MachineType::Mealy;
    }
    else if (argv[1] == MOORE_MODE)
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