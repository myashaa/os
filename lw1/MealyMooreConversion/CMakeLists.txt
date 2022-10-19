cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(MealyMooreConversion)

set(CMAKE_CXX_STANDARD 20)

add_executable(MealyMooreConversion main.cpp src/Machine/Machine.h src/Reader/Reader.h src/Reader/Reader.cpp src/Converter/Converter.h src/Converter/Converter.cpp src/Writer/Writer.h src/Writer/Writer.cpp)