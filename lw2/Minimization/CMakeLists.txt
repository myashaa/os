cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(Minimization)

set(CMAKE_CXX_STANDARD 20)

add_executable(Minimization main.cpp src/Machine/Machine.h src/Reader/Reader.h src/Reader/Reader.cpp src/Minimizer/Minimizer.h src/Minimizer/Minimizer.cpp src/Writer/Writer.h src/Writer/Writer.cpp)