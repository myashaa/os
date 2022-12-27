#include "CParser.h"
#include "CLineHandler.h"

bool CParser::Parse(ifstream& input, string& errorMessage)
{
	string str;

	if(!ParseProg(input, errorMessage, str))
	{
		return false;
	}

	if (!ParseVar(input, errorMessage, str))
	{
		return false;
	}

	if (!ParseBegin(input, errorMessage, str))
	{
		return false;
	}

	if (!ListSTParser(input, str, errorMessage))
	{
		return false;
	}

	if (!ParseEnd(errorMessage, str))
	{
		return false;
	}

	return true;
}

bool CParser::ParseProg(ifstream& input, string& errorMessage, string& str)
{
	CLineHandler::ReadLine(input, str);

	if (str != PROG)
	{
		errorMessage = Errors[PROG];
		return false;
	}

	return true;
}

bool CParser::ParseVar(ifstream& input, string& errorMessage, string& str)
{
	CLineHandler::ReadLine(input, str);

	if (!VarParser(str, errorMessage))
	{
		return false;
	}

	return true;
}

bool CParser::ParseBegin(ifstream& input, string& errorMessage, string& str)
{
	CLineHandler::ReadLine(input, str);

	if (str != BEGIN)
	{
		errorMessage = Errors[BEGIN];
		return false;
	}

	return true;
}

bool CParser::ParseEnd(string& errorMessage, string& str)
{
	if (str != END)
	{
		errorMessage = Errors[END];
		return false;
	}

	return true;
}



bool CParser::VarParser(string& str, string& errorMessage)
{
	bool result = false;

	string pars = CLineHandler::GetSubstring(str, SPACE);
	if (pars == VAR)
	{
		if (str.find(COLON) == str.npos)
		{
			errorMessage = Errors[COLON];
			return false;
		}

		pars = str.substr(pars.length() + 1, str.find(COLON) - 1 - pars.length());
		str = str.substr(str.find(COLON) + 1);

		if (IDListParser(pars, errorMessage) && TypeParser(str, errorMessage))
		{
			result = true;
		}
	}
	else
	{
		errorMessage = Errors[VAR];
	}

	return result;
}

bool CParser::IDListParser(string& str, string& errorMessage)
{
	CLineHandler::RemoveExtraSpaces(str);

	if (str == ID)
	{
		return true;
	}

	bool result = false;

	string pars = CLineHandler::GetSubstring(str, SPACE);
	if (pars == MANY_ID)
	{
		str = str.substr(pars.length());
		result = IDListParser(str, errorMessage);
	}
	else
	{
		if (pars == ID)
		{
			errorMessage = Errors[COMMA];
		}
		else
		{
			errorMessage = Errors[ID];
		}
	}

	return result;
}

bool CParser::TypeParser(string& str, string& errorMessage)
{
	if (str.find(SEMICOLON) != str.npos)
	{
		str = CLineHandler::GetSubstring(str, SEMICOLON);
	}
	else
	{
		errorMessage = Errors[SEMICOLON];
		return false;
	}

	bool result = false;

	if (str == INT || str == FLOAT || str == BOOL || str == STRING)
	{
		result = true;
	}
	else
	{
		errorMessage = Errors[DATA_TYPE];
	}

	str = EMPTY_STR;

	return result;
}

bool CParser::ListSTParser(ifstream& input, string& str, string& errorMessage)
{
	unsigned operationCount = 0;

	while (CLineHandler::ReadLine(input, str))
	{
		if (str == END)
		{
			if (operationCount > 0)
			{
				return true;
			}

			errorMessage = Errors[FUNCTIONS];
			return false;
		}

		if (str.find(SEMICOLON) == str.npos)
		{
			errorMessage = Errors[SEMICOLON];
			return false;
		}

		operationCount++;

		str = str.substr(0, str.find(SEMICOLON));

		if (STParser(str, errorMessage) == false)
		{
			return false;
		}
	}

	return true;
}

bool CParser::STParser(string const& str, string& errorMessage)
{
	string pars = str;
	if (ReadParser(pars, errorMessage))
	{
		return true;
	}

	pars = str;
	if (WriteParser(pars, errorMessage))
	{
		return true;
	}

	pars = str;
	if (AssignParser(pars, errorMessage))
	{
		return true;
	}

	if (errorMessage.empty())
	{
		errorMessage = Errors[UNKNOWN_FUNCTION];
	}

	return false;
}

bool CParser::ReadParser(string& str, string& errorMessage)
{
	return OperationParser(READ, str, errorMessage);
}

bool CParser::WriteParser(string& str, string& errorMessage)
{
	return OperationParser(WRITE, str, errorMessage);
}

bool CParser::AssignParser(string& str, string& errorMessage)
{
	if (str.find(EQUAL) == str.npos)
	{
		return false;
	}

	string pars = CLineHandler::GetSubstring(str, EQUAL);

	if (pars != ID)
	{
		errorMessage = Errors[ID];
		return false;
	}

	bool result = false;

	pars = str.substr(0, pars.length() + 3);
	CLineHandler::RemoveExtraSpaces(pars);

	if (pars == ID_EQUAL)
	{
		str = str.substr(pars.length());

		if (CLineHandler::IsCorrectBrackets(str, errorMessage))
		{
			result = EXPParser(str, errorMessage);
		}
	}
	else
	{
		errorMessage = Errors[EQUAL];
	}

	if (result == true)
	{
		errorMessage = "";
	}

	return result;
}

bool CParser::EXPParser(string& str, string& errorMessage)
{
	bool result = false;

	CLineHandler::RemoveExtraSpaces(str);

	string pars = str;
	if (TParser(pars, errorMessage))
	{
		str = EMPTY_STR;
		result = true;
	}
	else
	{
		pars = CLineHandler::GetSubstring(str, PLUS);

		if (TParser(pars, errorMessage))
		{
			str = str.substr(str.find(PLUS) + 1);
			result = EXPParser(str, errorMessage);
		}
	}

	return result;
}

bool CParser::TParser(string& str, string& errorMessage)
{
	bool result = false;

	CLineHandler::RemoveExtraSpaces(str);

	string pars = str;
	if (FParser(pars, errorMessage))
	{
		str = EMPTY_STR;
		result = true;
	}
	else
	{
		pars = CLineHandler::GetSubstring(str, MULTIPLY);

		if (FParser(pars, errorMessage))
		{
			str = str.substr(str.find(MULTIPLY) + 1);

			result = EXPParser(str, errorMessage);
		}
	}

	return result;
}

bool CParser::FParser(string& str, string& errorMessage)
{
	if (str == ID || str == NUM)
	{
		return true;
	}

	bool result = false;

	string pars = str.substr(0, 1);
	if (pars == OPEN_BRACKET_STR)
	{
		str = str.substr(pars.length());
		CLineHandler::RemoveExtraBrackets(str);

		result = EXPParser(str, errorMessage);
	}
	else if (pars == MINUS)
	{
		str = str.substr(pars.length());

		result = FParser(str, errorMessage);
	}

	if (result == false)
	{
		errorMessage = Errors[UNKNOWN_EXPRESSION];
	}

	return result;
}



bool CParser::OperationParser(string const& operation, string& str, string& errorMessage)
{
	bool result = false;

	string pars = CLineHandler::GetSubstring(str, OPEN_BRACKET_STR);

	if (pars == operation)
	{
		// Проверяем, что следующий символ - открывающая скобка.
		if (!CLineHandler::IsOperationHaveOpenBracket(str, pars))
		{
			errorMessage = Errors[OPEN_BRACKET_STR];
			return false;
		}

		str = str.substr(pars.length() + 1);

		// Если закрывающей скобки не обнаружено, выбрасываем ошибку.
		if (str.find(CLOSE_BRACKET_STR) == str.npos)
		{
			errorMessage = Errors[CLOSE_BRACKET_STR];
			return false;
		}

		pars = CLineHandler::GetSubstring(str, CLOSE_BRACKET_STR);

		result = IDListParser(pars, errorMessage);

		if (result == true)
		{
			str = str.substr(str.find(CLOSE_BRACKET_STR));

			// Если после закрывающей скобки есть что-то кроме ';' выкидываем ошибку.
			if (str.length() > 1)
			{
				errorMessage = Errors[SEMICOLON];
				result = false;
			}
		}
	}

	return result;
}