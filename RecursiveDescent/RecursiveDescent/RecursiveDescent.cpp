#include <string>
#include <fstream>

using namespace std;

// Вспомогательные методы +

void RemoveExtraSpaces(string& str)
{
	size_t begin = str.find_first_not_of(" ");
	size_t length = str.find_last_not_of(" ") - begin;

	str = str.substr(begin, length + 1);
}

string GetSubstring(string const& str, string const& separartor)
{
	string result = str.substr(0, str.find(separartor) + 1);

	RemoveExtraSpaces(result);

	return result;
}

void ReadLine(ifstream& input, string& str)
{
	str = "";

	if (!input.eof())
	{
		getline(input, str);
	}
}

// Вспомогательные методы -
bool EXPLexer(string& str);


// <TYPE> -> int | float | bool | string
bool TypeLexer(string& str)
{
	bool result = false;

	//string lex = GetSubstring(str, ";");
	str = str.substr(0, str.length() - 1);

	if (str == "int" || str == "float" || str == "bool" || str == "string")
	{
		result = true;
	}

	str = "";
	//str = str.substr(lex.length());

	return result;
}

// <IDLIST> -> id | <IDLIST>, id
bool IDListLexer(string& str)
{
	RemoveExtraSpaces(str);

	if (str == "id")
	{
		return true;
	}

	bool result = false;

	string lex = GetSubstring(str, " ");
	if (lex == "id,")
	{
		str = str.substr(lex.length());
		result = IDListLexer(str);
	}

	return result;
}

//<WRITE> -> WRITE(<IDLIST>);
bool WriteLexer(string& str)
{
	bool result = false;

	string lex = GetSubstring(str, "(");

	if (lex == "WRITE(")
	{
		str.substr(lex.length());

		result = IDListLexer(str);

		if (result == true)
		{
			result = (str[0] == ')');
		}
	}

	return result;
}

// <READ> -> READ(<IDLIST>);
bool ReadLexer(string& str)
{
	bool result = false;

	string lex = GetSubstring(str, "(");

	if (lex == "READ(")
	{
		str = str.substr(lex.length());

		lex = str.substr(0, str.find(")"));

		result = IDListLexer(lex);

		if (result == true)
		{
			str = str.substr(str.find(")"));
			//result = (str[0] == ')');
		}
	}

	return result;
}

// <F> -> -<F> | (<EXP>) | id | num
bool FLexer(string& str)
{
	if (str == "id" || str == "num")
	{
		return true;
	}

	bool result = false;

	string lex = str.substr(0, 1);
	if (lex == "(")
	{
		str.substr(lex.length());
		result = EXPLexer(str);

		if (result)
		{
			result = (str[0] == ')');
		}

		str = str.substr(1);
	}
	else if (lex == "-")
	{
		str.substr(lex.length());
		result = FLexer(str);
	}

	return result;
}

// TODO: Дописать, проверить работоспособность. Сначала <F>, потом <T>*<F>
// <T> -> <T>*<F> | <F>
bool TLexer(string& str)
{
	bool result = false;

	string lex = str;
	if (FLexer(lex))
	{
		result = true;
	}
	else
	{
		lex = GetSubstring(str, "*");
		if (FLexer(lex))
		{
			str.substr(lex.length());
			result = TLexer(str);
		}
	}

	return result;
}

// TODO: Дописать, нужно как то разбить строку для корректной обработки.
// <EXP> ->  <EXP> + <T> | <T>
bool EXPLexer(string& str)
{
	bool result = false;

	string lex = str;
	if (TLexer(lex))
	{
		result = true;
	}
	else
	{
		// Разделить по "+"
		lex = GetSubstring(str, "+");
		if (TLexer(lex))
		{
			str.substr(lex.length());
			result = EXPLexer(str);
		}
	}

	return result;
}

// <ASSIGN> -> id := <EXP> ;
bool AssignLexer(string& str)
{
	bool result = false;

	string lex = GetSubstring(str, ":=");

	if (lex == "id :=")
	{
		str = str.substr(lex.length());

		result = EXPLexer(str);
	}

	return result;
}

// <ST> -> <READ> | <WRITE> | <ASSIGN>
bool STLexer(string const& str)
{
	string lex = str;
	if (ReadLexer(lex))
	{
		return true;
	}

	lex = str;
	if (WriteLexer(lex))
	{
		return true;
	}

	lex = str;
	if (AssignLexer(lex))
	{
		return true;
	}

	return false;
}

// <LISTST> -> <ST> | <LISTST> <ST>
bool ListSTLexer(ifstream& input)
{
	bool result = false;

	string str;
	while (getline(input, str) && str != "END")
	{
		result = STLexer(str);

		if (!result)
		{
			break;
		}
	}

	return result;
}

// <VAR> -> VAR <IDLIST> : <TYPE>
bool VarLexer(string& str)
{
	bool result = false;

	string lex = GetSubstring(str, " ");
	if (lex == "VAR")
	{
		lex = str.substr(lex.length(), str.find(":") - lex.length());
		str = str.substr(str.find(":") + 2);

		if (IDListLexer(lex) && TypeLexer(str))
		{
			result = true;
		}
	}

	return result;
}

// <PROG> -> PROG id <VAR> begin <LISTST> end
bool ProgLexer(ifstream& input)
{
	string str;

	ReadLine(input, str); //getline(input, str);
	if (str != "PROG id")
	{
		return false;
	}

	ReadLine(input, str);
	if (!VarLexer(str))
	{
		return false;
	}

	ReadLine(input, str);
	if (str != "BEGIN")
	{
		return false;
	}

	if (!ListSTLexer(input))
	{
		return false;
	}

	ReadLine(input, str);
	if (str != "END")
	{
		return false;
	}

	return true;
}

int main(int argc, char* argv[])
{
	ifstream input(argv[1]);

	ofstream out("output.txt");

	if (ProgLexer(input))
	{
		out << "Ok";
	}
	else
	{
		out << "Err";
	}

	return 0;
}