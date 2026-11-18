#include "ConsoleRenderer.h"

#include <iomanip>

ConsoleRenderer::ConsoleRenderer(char alive, char dead)
    : m_alive(alive), m_dead(dead)
{
}

void ConsoleRenderer::draw(const Board& board, std::ostream& out) const
{
    const BitGrid& grid = board.grid();
    for (int y = 0; y < grid.height(); ++y)
    {
        for (int x = 0; x < grid.width(); ++x)
        {
            out << (grid.get(x, y) ? m_alive : m_dead);
        }
        out << '\n';
    }
    out << "Generation: " << board.generation()
        << "  Population: " << board.population()
        << "  Rule: " << board.rule().notation()
        << "  Wrap: " << (board.wrap() ? "on" : "off") << '\n';
}

void ConsoleRenderer::drawBits(const BitGrid& grid, std::ostream& out, std::size_t maxBytes) const
{
    const std::vector<std::uint8_t>& bytes = grid.bytes();
    out << "Bit order: " << Endian::name(grid.bitOrder())
        << ", total bytes: " << bytes.size() << '\n';

    const std::size_t limit = bytes.size() < maxBytes ? bytes.size() : maxBytes;
    for (std::size_t i = 0; i < limit; ++i)
    {
        out << "byte " << std::setw(3) << i << ": 0x"
            << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(bytes[i])
            << std::dec << std::setfill(' ') << "  ";
        for (int bit = 7; bit >= 0; --bit)
        {
            out << ((bytes[i] >> bit) & 1);
        }
        out << '\n';
    }
    if (limit < bytes.size())
    {
        out << "... " << (bytes.size() - limit) << " more bytes\n";
    }
}

void ConsoleRenderer::clearScreen(std::ostream& out) const
{
    out << "\033[2J\033[H";
}
