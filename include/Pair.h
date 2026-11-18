#pragma once

template <typename T1, typename T2>
class Pair
{
public:
    Pair(const T1& first, const T2& second)
        : m_first(first), m_second(second)
    {
    }

    const T1& first() const { return m_first; }
    const T2& second() const { return m_second; }

private:
    T1 m_first;
    T2 m_second;
};
