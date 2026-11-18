#include "Board.h"

#include <random>
#include <utility>

Board::Board(Pair1<int> size, Rule rule, bool wrap, BitOrder order)
    : m_grid(size, order), m_rule(rule), m_wrap(wrap), m_generation(0)
{
}

Board::Board(BitGrid grid, Rule rule, bool wrap, std::size_t generation)
    : m_grid(std::move(grid)), m_rule(rule), m_wrap(wrap), m_generation(generation)
{
}

int Board::neighbors(int x, int y) const
{
    const int w = m_grid.width();
    const int h = m_grid.height();
    int total = 0;

    for (int dy = -1; dy <= 1; ++dy)
    {
        for (int dx = -1; dx <= 1; ++dx)
        {
            if (dx == 0 && dy == 0)
            {
                continue;
            }
            int nx = x + dx;
            int ny = y + dy;
            if (m_wrap)
            {
                nx = (nx + w) % w;
                ny = (ny + h) % h;
            }
            else if (nx < 0 || ny < 0 || nx >= w || ny >= h)
            {
                continue;
            }
            if (m_grid.get(nx, ny))
            {
                ++total;
            }
        }
    }
    return total;
}

void Board::step()
{
    BitGrid next(m_grid.size(), m_grid.bitOrder());
    for (int y = 0; y < m_grid.height(); ++y)
    {
        for (int x = 0; x < m_grid.width(); ++x)
        {
            next.set(x, y, m_rule.next(m_grid.get(x, y), neighbors(x, y)));
        }
    }
    m_grid = std::move(next);
    ++m_generation;
}

void Board::clear()
{
    m_grid.clear();
    m_generation = 0;
}

void Board::randomize(unsigned seed, double density)
{
    std::mt19937 engine(seed);
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    for (int y = 0; y < m_grid.height(); ++y)
    {
        for (int x = 0; x < m_grid.width(); ++x)
        {
            m_grid.set(x, y, dist(engine) < density);
        }
    }
    m_generation = 0;
}

void Board::place(const Pattern& pattern, int offsetX, int offsetY)
{
    const int w = m_grid.width();
    const int h = m_grid.height();
    for (const Pair<int, int>& cell : pattern.cells())
    {
        int x = offsetX + cell.first();
        int y = offsetY + cell.second();
        if (m_wrap)
        {
            x = ((x % w) + w) % w;
            y = ((y % h) + h) % h;
        }
        else if (x < 0 || y < 0 || x >= w || y >= h)
        {
            continue;
        }
        m_grid.set(x, y, true);
    }
}

void Board::setBitOrder(BitOrder order)
{
    m_grid = m_grid.withBitOrder(order);
}
