#pragma once

#include <cstddef>
#include <ostream>

#include "Board.h"

class ConsoleRenderer
{
public:
    explicit ConsoleRenderer(char alive = '#', char dead = '.');

    void draw(const Board& board, std::ostream& out) const;
    void drawBits(const BitGrid& grid, std::ostream& out, std::size_t maxBytes = 16) const;
    void clearScreen(std::ostream& out) const;

private:
    char m_alive;
    char m_dead;
};
