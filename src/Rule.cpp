#include "Rule.h"

#include <cctype>
#include <stdexcept>

Rule::Rule()
    : m_birth(static_cast<std::uint16_t>(1u << 3)),
      m_survive(static_cast<std::uint16_t>((1u << 2) | (1u << 3)))
{
}

Rule::Rule(std::uint16_t birthMask, std::uint16_t surviveMask)
    : m_birth(birthMask), m_survive(surviveMask)
{
}

Rule::Rule(const std::string& notation)
    : m_birth(0), m_survive(0)
{
    int mode = 0;
    bool hasBirth = false;
    bool hasSurvive = false;

    for (char c : notation)
    {
        if (c == 'B' || c == 'b')
        {
            mode = 1;
            hasBirth = true;
        }
        else if (c == 'S' || c == 's')
        {
            mode = 2;
            hasSurvive = true;
        }
        else if (c == '/' || std::isspace(static_cast<unsigned char>(c)))
        {
            continue;
        }
        else if (c >= '0' && c <= '8')
        {
            if (mode == 0)
            {
                throw std::invalid_argument("Invalid rule notation");
            }
            const std::uint16_t bit = static_cast<std::uint16_t>(1u << (c - '0'));
            if (mode == 1)
            {
                m_birth = static_cast<std::uint16_t>(m_birth | bit);
            }
            else
            {
                m_survive = static_cast<std::uint16_t>(m_survive | bit);
            }
        }
        else
        {
            throw std::invalid_argument("Invalid rule notation");
        }
    }

    if (!hasBirth || !hasSurvive)
    {
        throw std::invalid_argument("Rule must contain both B and S parts");
    }
}

bool Rule::next(bool alive, int neighbors) const
{
    if (neighbors < 0 || neighbors > 8)
    {
        return false;
    }
    const std::uint16_t bit = static_cast<std::uint16_t>(1u << neighbors);
    return alive ? (m_survive & bit) != 0 : (m_birth & bit) != 0;
}

std::string Rule::notation() const
{
    std::string result = "B";
    for (int i = 0; i <= 8; ++i)
    {
        if (m_birth & (1u << i))
        {
            result += static_cast<char>('0' + i);
        }
    }
    result += "/S";
    for (int i = 0; i <= 8; ++i)
    {
        if (m_survive & (1u << i))
        {
            result += static_cast<char>('0' + i);
        }
    }
    return result;
}
