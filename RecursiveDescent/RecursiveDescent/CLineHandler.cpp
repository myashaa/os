#include "CLineHandler.h"

bool CLineHandler::ReadLine(ifstream& input, string& str)
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

void CLineHandler::RemoveExtraSpaces(string& str)
{
	size_t begin = str.find_first_not_of(SPACE);
	size_t length = str.find_last_not_of(SPACE) - begin;

	if (begin == str.npos)
	{
		begin = 0;
	}

	str = str.substr(begin, length + 1);
}

string CLineHandler::GetSubstring(string const& str, string const& separartor)
{
	string result = str.substr(0, str.find(separartor));

	RemoveExtraSpaces(result);

	return result;
}

bool CLineHandler::IsCorrectBrackets(string const& str, string& errorMessage)
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

void CLineHandler::RemoveExtraBrackets(string& str)
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

bool CLineHandler::IsOperationHaveOpenBracket(string const& str, string const& operation)
{
	bool haveOpenBracket = true;

	string pars = str.substr(0, operation.length() + 1);
	if (pars != operation + OPEN_BRACKET_STR)
	{
		haveOpenBracket = false;
	}

	return haveOpenBracket;
}

void CLineHandler::ToLower(string& str)
{
	transform(str.begin(), str.end(), str.begin(), [](unsigned char ch) 
		{return tolower(ch);}
	);
}