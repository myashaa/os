#pragma once
#include <vector>

struct Rule
{
    Rule(char term,
        std::vector<char> guidSimbol,
        bool shift,
        bool error,
        int ptrNext,
        bool isNextInStack,
        bool isEnd)
    {
        m_term = term;
        m_guidSimbol = guidSimbol;
        m_shift = shift;
        m_error = error;
        m_ptrNext = ptrNext;
        m_isNextInStack = isNextInStack;
        m_isEnd = isEnd;
    }

    char m_term;
    std::vector<char> m_guidSimbol;
    bool m_shift;
    bool m_error;
    int m_ptrNext;
    bool m_isNextInStack;
    bool m_isEnd;
};
