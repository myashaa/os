#pragma once
#include <map>

#include "Const.h"

std::map<std::string, std::string> Errors =
{
    {ID,                 "Expected 'id'\n"},
    {FUNCTIONS,          "Expected operation 'READ|WRITE|ASSIGN'\n"},
    {PROG,               "Expected 'PROG id'\n"},
    {BEGIN,              "Expected 'BEGIN'\n"},
    {END,                "Expected 'END'\n"},
    {PLUS,               "Expected '+'\n"}, // Не используется
    {MULTIPLY,           "Expected '*'\n"}, // Не используется
    {COLON,              "Expected ':'\n"},
    {VAR,                "Expected 'VAR'\n"},
    {DATA_TYPE,          "Expected data type 'int|float|bool|string'\n"},
    {OPEN_BRACKET_STR,   "Expected '('\n"},
    {CLOSE_BRACKET_STR,  "Expected ')'\n"},
    {EQUAL,              "Expected ':='\n"},
    {COMMA,              "Expected ','\n"},
    {UNKNOWN_FUNCTION,   "Found unknown function\n"},
    {SEMICOLON,          "Expected ';'\n"},
    {UNKNOWN_EXPRESSION, "Unknown expression\n"}
};