#include "BitGrid.h"

#include <stdexcept>
#include <utility>

namespace
{
void validate(const Pair1<int>& size)
{
    if (size.first() <= 0 || size.second() <= 0)
    {
        throw std::invalid_argument("Grid dimensions must be positive");
    }
}
}

std::size_t BitGrid::byteCount(Pair1<int> size)
{
    validate(size);
    const std::size_t bits = static_cast<std::size_t>(size.first()) * static_cast<std::size_t>(size.second());
    return (bits + 7) / 8;
}

BitGrid::BitGrid(Pair1<int> size, BitOrder order)
    : m_width(size.first()),
      m_height(size.second()),
      m_order(order),
      m_bytes(byteCount(size), 0)
{
}

BitGrid::BitGrid(Pair1<int> size, BitOrder order, std::vector<std::uint8_t> bytes)
    : m_width(size.first()),
      m_height(size.second()),
      m_order(order),
      m_bytes(std::move(bytes))
{
    if (m_bytes.size() != byteCount(size))
    {
        throw std::invalid_argument("Byte buffer size does not match grid dimensions");
    }
}

std::size_t BitGrid::index(int x, int y) const
{
    if (x < 0 || y < 0 || x >= m_width || y >= m_height)
    {
        throw std::out_of_range("Cell coordinates are out of range");
    }
    return static_cast<std::size_t>(y) * static_cast<std::size_t>(m_width) + static_cast<std::size_t>(x);
}

std::uint8_t BitGrid::mask(std::size_t bitInByte) const
{
    if (m_order == BitOrder::LsbFirst)
    {
        return static_cast<std::uint8_t>(1u << bitInByte);
    }
    return static_cast<std::uint8_t>(0x80u >> bitInByte);
}

bool BitGrid::get(int x, int y) const
{
    const std::size_t i = index(x, y);
    return (m_bytes[i / 8] & mask(i % 8)) != 0;
}

void BitGrid::set(int x, int y, bool alive)
{
    const std::size_t i = index(x, y);
    const std::uint8_t bit = mask(i % 8);
    if (alive)
    {
        m_bytes[i / 8] = static_cast<std::uint8_t>(m_bytes[i / 8] | bit);
    }
    else
    {
        m_bytes[i / 8] = static_cast<std::uint8_t>(m_bytes[i / 8] & ~bit);
    }
}

void BitGrid::clear()
{
    for (std::uint8_t& b : m_bytes)
    {
        b = 0;
    }
}

std::size_t BitGrid::count() const
{
    std::size_t total = 0;
    for (std::uint8_t b : m_bytes)
    {
        while (b)
        {
            total += b & 1u;
            b = static_cast<std::uint8_t>(b >> 1);
        }
    }
    return total;
}

BitGrid BitGrid::withBitOrder(BitOrder order) const
{
    if (order == m_order)
    {
        return *this;
    }
    std::vector<std::uint8_t> converted(m_bytes.size());
    for (std::size_t i = 0; i < m_bytes.size(); ++i)
    {
        converted[i] = Endian::reverseBits(m_bytes[i]);
    }
    return BitGrid(size(), order, std::move(converted));
}
