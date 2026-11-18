#include "Pattern.h"

Pattern::Pattern(const std::string& name, const CellList& cells)
    : StringValuePair<CellList>(name, cells)
{
}

PatternLibrary::PatternLibrary()
{
    m_patterns.push_back(Pattern("block", {{0, 0}, {1, 0}, {0, 1}, {1, 1}}));
    m_patterns.push_back(Pattern("blinker", {{0, 0}, {1, 0}, {2, 0}}));
    m_patterns.push_back(Pattern("toad", {{1, 0}, {2, 0}, {3, 0}, {0, 1}, {1, 1}, {2, 1}}));
    m_patterns.push_back(Pattern("beacon", {{0, 0}, {1, 0}, {0, 1}, {3, 2}, {2, 3}, {3, 3}}));
    m_patterns.push_back(Pattern("glider", {{1, 0}, {2, 1}, {0, 2}, {1, 2}, {2, 2}}));
    m_patterns.push_back(Pattern("lwss", {{1, 0}, {4, 0}, {0, 1}, {0, 2}, {4, 2}, {0, 3}, {1, 3}, {2, 3}, {3, 3}}));
    m_patterns.push_back(Pattern("rpentomino", {{1, 0}, {2, 0}, {0, 1}, {1, 1}, {1, 2}}));
    m_patterns.push_back(Pattern("gosper", {
        {1, 5}, {2, 5}, {1, 6}, {2, 6},
        {11, 5}, {11, 6}, {11, 7}, {12, 4}, {12, 8}, {13, 3}, {13, 9},
        {14, 3}, {14, 9}, {15, 6}, {16, 4}, {16, 8}, {17, 5}, {17, 6},
        {17, 7}, {18, 6}, {21, 3}, {21, 4}, {21, 5}, {22, 3}, {22, 4},
        {22, 5}, {23, 2}, {23, 6}, {25, 1}, {25, 2}, {25, 6}, {25, 7},
        {35, 3}, {35, 4}, {36, 3}, {36, 4}}));
}

const Pattern* PatternLibrary::find(const std::string& name) const
{
    for (const Pattern& p : m_patterns)
    {
        if (p.name() == name)
        {
            return &p;
        }
    }
    return nullptr;
}
