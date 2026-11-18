#pragma once

#include <cstdint>
#include <string>

class Rule
{
public:
    Rule();
    Rule(std::uint16_t birthMask, std::uint16_t surviveMask);
    explicit Rule(const std::string& notation);

    bool next(bool alive, int neighbors) const;
    std::string notation() const;

    std::uint16_t birthMask() const { return m_birth; }
    std::uint16_t surviveMask() const { return m_survive; }

private:
    std::uint16_t m_birth;
    std::uint16_t m_survive;
};
