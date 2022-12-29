#include "CHandler.h"

void CHandler::InitializeArr(vector<Rule>& arr)
{
    arr.push_back(Rule('S', { '8', '3', 'a', 'b', '-', '(' }, false, true, 1, false, false));
    arr.push_back(Rule('E', { '8', '3', 'a', 'b', '-', '(' }, false, true, 3, true, false));
    arr.push_back(Rule('\0', { '\0' }, true, true, -1, false, true));
    arr.push_back(Rule('E', { '8', '3', 'a', 'b', '-', '(' }, false, true, 4, false, false));
    arr.push_back(Rule('T', { '8', '3', 'a', 'b', '-', '(' }, false, true, 14, true, false));
    arr.push_back(Rule('A', { ')', '+', '\0' }, false, true, 6, false, false));
    arr.push_back(Rule('A', { ')' }, false, false, 10, false, false));
    arr.push_back(Rule('A', { '+' }, false, false, 11, false, false));
    arr.push_back(Rule('A', { '\0' }, false, true, 9, false, false));
    arr.push_back(Rule('\0', { '\0' }, false, true, -1, false, false));
    arr.push_back(Rule(')', { ')' }, false, true, -1, false, false));
    arr.push_back(Rule('+', { '+' }, true, true, 12, false, false));
    arr.push_back(Rule('T', { '8', '3', 'a', 'b', '-', '(' }, false, true, 14, true, false));
    arr.push_back(Rule('A', { ')', '+', '\0' }, false, true, 6, false, false));
    arr.push_back(Rule('T', { '8', '3', 'a', 'b', '-', '(' }, false, true, 15, false, false));
    arr.push_back(Rule('F', { '8', '3', 'a', 'b', '-', '(' }, false, true, 27, true, false));
    arr.push_back(Rule('B', { '*', ')', '+', '\0' }, false, true, 17, false, false));
    arr.push_back(Rule('B', { '*' }, false, false, 24, false, false));
    arr.push_back(Rule('B', { ')' }, false, false, 22, false, false));
    arr.push_back(Rule('B', { '+' }, false, false, 23, false, false));
    arr.push_back(Rule('B', { '\0' }, false, true, 21, false, false));
    arr.push_back(Rule('\0', { '\0' }, false, true, -1, false, false));
    arr.push_back(Rule(')', { ')' }, false, true, -1, false, false));
    arr.push_back(Rule('+', { '+' }, false, true, -1, false, false));
    arr.push_back(Rule('*', { '*' }, true, true, 25, false, false));
    arr.push_back(Rule('F', { '8', '3', 'a', 'b', '-', '(' }, false, true, 27, true, false));
    arr.push_back(Rule('B', { '*', ')', '+', '\0' }, false, true, 17, false, false));
    arr.push_back(Rule('F', { '8' }, false, false, 33, false, false));
    arr.push_back(Rule('F', { '3' }, false, false, 34, false, false));
    arr.push_back(Rule('F', { 'a' }, false, false, 35, false, false));
    arr.push_back(Rule('F', { 'b' }, false, false, 36, false, false));
    arr.push_back(Rule('F', { '-' }, false, false, 37, false, false));
    arr.push_back(Rule('F', { '(' }, false, true, 39, false, false));
    arr.push_back(Rule('8', { '8' }, true, true, -1, false, false));
    arr.push_back(Rule('3', { '3' }, true, true, -1, false, false));
    arr.push_back(Rule('a', { 'a' }, true, true, -1, false, false));
    arr.push_back(Rule('b', { 'b' }, true, true, -1, false, false));
    arr.push_back(Rule('-', { '-' }, true, true, 38, false, false));
    arr.push_back(Rule('F', { '8', '3', 'a', 'b', '-', '(' }, false, true, 27, false, false));
    arr.push_back(Rule('(', { '(' }, true, true, 40, false, false));
    arr.push_back(Rule('E', { '8', '3', 'a', 'b', '-', '(' }, false, true, 3, true, false));
    arr.push_back(Rule(')', { ')' }, true, true, -1, false, false));
}

bool CHandler::CompareLine(vector<Rule> const& arr, string line)
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