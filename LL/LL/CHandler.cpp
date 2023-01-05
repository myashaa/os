#include "CHandler.h"

using namespace std;

void CHandler::RemoveExtraSpaces(string& str)
{
    size_t begin = str.find_first_not_of(' ');
    size_t length = str.find_last_not_of(' ') - begin;

    if (begin == str.npos)
    {
        begin = 0;
    }

    str = str.substr(begin, length + 1);
}

string CHandler::ReadStringPart(string& str, char separator)
{
    string part = str.substr(0, str.find(separator));

    str = str.substr(part.size());

    RemoveExtraSpaces(str);

    return part;
}

void CHandler::InitializeArr(ifstream& table, vector<Rule>& arr)
{
    arr.clear();

    string row;
    while (getline(table, row))
    {
        char term = row[0];

        string strPart = row.substr(row.find('{') + 1, row.find('}') - row.find('{') - 1);
        vector<char> guidChars;

        for (size_t i = 0; i < strPart.size(); i++)
        {
            char ch = strPart[i];

            if (ch != ',' && ch != ' ')
            {
                if (ch == '.')
                {
                    ch = '\0';
                }

                guidChars.push_back(ch);
            }
        }

        row = row.substr(row.find('}') + 2);

        strPart = ReadStringPart(row, ' ');
        bool shift = (strPart == "true");

        strPart = ReadStringPart(row, ' ');
        bool error = (strPart == "true");

        strPart = ReadStringPart(row, ' ');
        int ptrNext = stoi(strPart);

        strPart = ReadStringPart(row, ' ');
        bool isNextInStack = (strPart == "true");

        strPart = ReadStringPart(row, ' ');
        bool isEnd = (strPart == "true");

        arr.push_back(Rule(term, guidChars, shift, error, ptrNext, isNextInStack, isEnd));
    }
}

bool CHandler::CompareLine(vector<Rule> const& arr, string const& line)
{
    stack<int> nextStep;
    bool isSuccess = true;
    int index = 0;
    int ch = 0;

    while (isSuccess)
    {
        char simbol;
        simbol = (ch >= line.size()) ? '\0' : line[ch];

        auto found = find(arr[index].m_guidSimbol.begin(), arr[index].m_guidSimbol.end(), simbol);
        if (arr[index].m_error && found == arr[index].m_guidSimbol.end())
        {
            return false;
        }
        if (!arr[index].m_error && found == arr[index].m_guidSimbol.end())
        {
            ++index;
            continue;
        }
        if (arr[index].m_isNextInStack)
        {
            nextStep.push(index + 1);
        }
        if (arr[index].m_shift)
        {
            ++ch;
            simbol = (ch >= line.size()) ? '\0' : line[ch];
        }
        if (arr[index].m_isEnd)
        {
            break;
        }
        if (arr[index].m_ptrNext != -1)
        {
            index = arr[index].m_ptrNext;
        }
        else
        {
            index = nextStep.top();
            nextStep.pop();
        }
    }

    return isSuccess;
}