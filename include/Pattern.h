#pragma once

#include <string>
#include <vector>

#include "Pair.h"
#include "StringValuePair.h"

using CellList = std::vector<Pair<int, int>>;

class Pattern : public StringValuePair<CellList>
{
public:
    Pattern(const std::string& name, const CellList& cells);

    const std::string& name() const { return first(); }
    const CellList& cells() const { return second(); }
};

class PatternLibrary
{
public:
    PatternLibrary();

    const Pattern* find(const std::string& name) const;
    const std::vector<Pattern>& all() const { return m_patterns; }

private:
    std::vector<Pattern> m_patterns;
};
