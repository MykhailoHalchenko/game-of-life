#pragma once

#include <cstddef>

#include "BitGrid.h"
#include "Endian.h"
#include "Pair1.h"
#include "Pattern.h"
#include "Rule.h"

class Board
{
public:
    Board(Pair1<int> size, Rule rule = Rule(), bool wrap = true, BitOrder order = BitOrder::LsbFirst);
    Board(BitGrid grid, Rule rule, bool wrap, std::size_t generation);

    void step();
    void clear();
    void randomize(unsigned seed, double density);
    void place(const Pattern& pattern, int offsetX, int offsetY);
    void setBitOrder(BitOrder order);
    void setRule(const Rule& rule) { m_rule = rule; }
    void setWrap(bool wrap) { m_wrap = wrap; }

    int neighbors(int x, int y) const;
    std::size_t population() const { return m_grid.count(); }
    std::size_t generation() const { return m_generation; }

    const BitGrid& grid() const { return m_grid; }
    const Rule& rule() const { return m_rule; }
    bool wrap() const { return m_wrap; }

private:
    BitGrid m_grid;
    Rule m_rule;
    bool m_wrap;
    std::size_t m_generation;
};
