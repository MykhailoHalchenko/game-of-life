#pragma once

#include <string>

#include "Pair.h"

template <typename V>
class StringValuePair : public Pair<std::string, V>
{
public:
    StringValuePair(const std::string& key, const V& value)
        : Pair<std::string, V>(key, value)
    {
    }
};
