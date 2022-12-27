#include <fstream>
#include <stack>
#include <algorithm>

#include "Exceptions.h"

using namespace std;

// Вспомогательные методы +

void ToLower(string& str)
{
	transform(str.begin(), str.end(), str.begin(), [](unsigned char ch) {return tolower(ch);});
}

void RemoveExtraSpaces(string& str)
{
	size_t begin = str.find_first_not_of(SPACE);
	size_t length = str.find_last_not_of(SPACE) - begin;

	if (begin == str.npos)
	{
		begin = 0;
	}

	str = str.substr(begin, length + 1);
}

void RemoveExtraBrackets(string& str)
{
	stack<char> brackets;

	string result;

	for (size_t i = 0; i < str.length(); i++)
	{
		bool add = true;

		if (str[i] == OPEN_BRACKET)
		{
			brackets.push(str[i]);
		}
		if (str[i] == CLOSE_BRACKET)
		{
			if (brackets.empty())
			{
				add = false;
			}
			else
			{
				brackets.pop();
			}
		}

		if (add)
		{
			result = result + str[i];
		}
	}

	str = result;
}

bool CorrectBrackets(string const& str, string& errorMessage)
{
	stack<char> brackets;

	for (size_t i = 0; i < str.length(); i++)
	{
		if (str[i] == OPEN_BRACKET)
		{
			brackets.push(str[i]);
		}
		if (str[i] == CLOSE_BRACKET)
		{
			if (brackets.empty())
			{
				errorMessage = Errors[OPEN_BRACKET_STR];
				return false;
			}

			brackets.pop();
		}
	}

	if (!brackets.empty())
	{
		errorMessage = Errors[CLOSE_BRACKET_STR];
	}

	return brackets.empty();
}

string GetSubstring(string const& str, string const& separartor)
{
	string result = str.substr(0, str.find(separartor));

	RemoveExtraSpaces(result);

	return result;
}

bool ReadLine(ifstream& input, string& str)
{
	str = EMPTY_STR;

	if (!input.eof())
	{
		getline(input, str);
	}

	ToLower(str);
	RemoveExtraSpaces(str);

	return !str.empty();
}

bool OperationHaveOpenBracket(string const& str, string const& operation)
{
	bool haveOpenBracket = true;

	string lex = str.substr(0, operation.length() + 1);
	if (lex != operation + OPEN_BRACKET_STR)
	{
		haveOpenBracket = false;
	}

	return haveOpenBracket;
}

// Вспомогательные методы -

bool EXPLexer(string& str, string& errorMessage);

// <TYPE> -> int | float | bool | string
bool TypeLexer(string& str, string& errorMessage)
{
	if (str.find(SEMICOLON) != str.npos)
	{
		str = GetSubstring(str, SEMICOLON);
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

// <IDLIST> -> id | <IDLIST>, id
bool IDListLexer(string& str, string& errorMessage)
{
	RemoveExtraSpaces(str);

	if (str == ID)
	{
		return true;
	}

	bool result = false;

	string lex = GetSubstring(str, SPACE);
	if (lex == MANY_ID)
	{
		str = str.substr(lex.length());
		result = IDListLexer(str, errorMessage);
	}
	else
	{
		if (lex == ID)
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

bool OperationLexer(string const& operation, string& str, string& errorMessage)
{
	bool result = false;

	string lex = GetSubstring(str, OPEN_BRACKET_STR);

	if (lex == operation)
	{
		// Проверяем, что следующий символ - открывающая скобка.
		if (!OperationHaveOpenBracket(str, lex))
		{
			errorMessage = Errors[OPEN_BRACKET_STR];
			return false;
		}

		str = str.substr(lex.length() + 1);

		// Если закрывающей скобки не обнаружено, выбрасываем ошибку.
		if (str.find(CLOSE_BRACKET_STR) == str.npos)
		{
			errorMessage = Errors[CLOSE_BRACKET_STR];
			return false;
		}

		lex = GetSubstring(str, CLOSE_BRACKET_STR);

		result = IDListLexer(lex, errorMessage);

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

//<WRITE> -> WRITE(<IDLIST>);
bool WriteLexer(string& str, string& errorMessage)
{
	return OperationLexer(WRITE, str, errorMessage);
}

// <READ> -> READ(<IDLIST>);
bool ReadLexer(string& str, string& errorMessage)
{
	return OperationLexer(READ, str, errorMessage);
}

// <F> -> -<F> | (<EXP>) | id | num
bool FLexer(string& str, string& errorMessage)
{
	if (str == ID || str == NUM)
	{
		return true;
	}

	bool result = false;

	string lex = str.substr(0, 1);
	if (lex == OPEN_BRACKET_STR)
	{
		// Открываем скобки.
		str = str.substr(lex.length());
		RemoveExtraBrackets(str);

		result = EXPLexer(str, errorMessage);
	}
	else if (lex == MINUS)
	{
		str = str.substr(lex.length());

		result = FLexer(str, errorMessage);
	}

	if (result == false)
	{
		errorMessage = Errors[UNKNOWN_EXPRESSION];
	}

	return result;
}

// <T> -> <T>*<F> | <F>
bool TLexer(string& str, string& errorMessage)
{
	bool result = false;

	RemoveExtraSpaces(str);

	string lex = str;
	if (FLexer(lex, errorMessage))
	{
		str = EMPTY_STR;
		result = true;
	}
	else
	{
		// Разделить по '*'.
		lex = GetSubstring(str, MULTIPLY);

		if (FLexer(lex, errorMessage))
		{
			str = str.substr(str.find(MULTIPLY) + 1);

			result = EXPLexer(str, errorMessage);
		}
	}

	return result;
}

// <EXP> ->  <EXP> + <T> | <T>
bool EXPLexer(string& str, string& errorMessage)
{
	bool result = false;

	RemoveExtraSpaces(str);

	string lex = str;
	if (TLexer(lex, errorMessage))
	{
		str = EMPTY_STR;
		result = true;
	}
	else
	{
		// Разделить по '+'.
		lex = GetSubstring(str, PLUS);

		if (TLexer(lex, errorMessage))
		{
			str = str.substr(str.find(PLUS) + 1);
			result = EXPLexer(str, errorMessage);
		}
	}

	return result;
}

// <ASSIGN> -> id := <EXP> ;
bool AssignLexer(string& str, string& errorMessage)
{
	if (str.find(EQUAL) == str.npos)
	{
		return false;
	}

	string lex = GetSubstring(str, EQUAL);

	if (lex != ID)
	{
		errorMessage = Errors[ID];
		return false;
	}

	bool result = false;

	lex = str.substr(0, lex.length() + 3);
	RemoveExtraSpaces(lex);

	if (lex == ID_EQUAL)
	{
		str = str.substr(lex.length());

		if (CorrectBrackets(str, errorMessage))
		{
			result = EXPLexer(str, errorMessage);
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

// <ST> -> <READ> | <WRITE> | <ASSIGN>
bool STLexer(string const& str, string& errorMessage)
{
	string lex = str;
	if (ReadLexer(lex, errorMessage))
	{
		return true;
	}

	lex = str;
	if (WriteLexer(lex, errorMessage))
	{
		return true;
	}

	lex = str;
	if (AssignLexer(lex, errorMessage))
	{
		return true;
	}

	if (errorMessage.empty())
	{
		errorMessage = Errors[UNKNOWN_FUNCTION];
	}

	return false;
}

// <LISTST> -> <ST> | <LISTST> <ST>
bool ListSTLexer(ifstream& input, string& str, string& errorMessage)
{
	unsigned operationCount = 0;

	while (ReadLine(input, str))
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

		if (STLexer(str, errorMessage) == false)
		{
			return false;
		}
	}

	return true;
}

// <VAR> -> VAR <IDLIST> : <TYPE>
bool VarLexer(string& str, string& errorMessage)
{
	bool result = false;

	string lex = GetSubstring(str, SPACE);
	if (lex == VAR)
	{
		if (str.find(COLON) == str.npos)
		{
			errorMessage = Errors[COLON];
			return false;
		}

		lex = str.substr(lex.length() + 1, str.find(COLON) - 1 - lex.length());
		str = str.substr(str.find(COLON) + 1);

		if (IDListLexer(lex, errorMessage) && TypeLexer(str, errorMessage))
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

// <PROG> -> PROG id <VAR> begin <LISTST> end
bool ProgLexer(ifstream& input, string& errorMessage)
{
	string str;

	ReadLine(input, str);
	if (str != PROG)
	{
		errorMessage = Errors[PROG];
		return false;
	}

	ReadLine(input, str);
	if (!VarLexer(str, errorMessage))
	{
		return false;
	}

	ReadLine(input, str);
	if (str != BEGIN)
	{
		errorMessage = Errors[BEGIN];
		return false;
	}

	if (!ListSTLexer(input, str, errorMessage))
	{
		return false;
	}

	if (str != END)
	{
		errorMessage = Errors[END];
		return false;
	}

	return true;
}

int main(int argc, char* argv[])
{
	ifstream input(argv[1]);

	ofstream out("output.txt");

	string errorMessage;

	if (ProgLexer(input, errorMessage))
	{
		out << "Ok";
	}
	else
	{
		out << errorMessage << endl;
	}

	return 0;
}