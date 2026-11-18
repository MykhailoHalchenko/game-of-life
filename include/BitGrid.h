#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "Endian.h"
#include "Pair1.h"

class BitGrid
{
public:
    BitGrid(Pair1<int> size, BitOrder order = BitOrder::LsbFirst);
    BitGrid(Pair1<int> size, BitOrder order, std::vector<std::uint8_t> bytes);

    int width() const { return m_width; }
    int height() const { return m_height; }
    Pair1<int> size() const { return Pair1<int>(m_width, m_height); }
    BitOrder bitOrder() const { return m_order; }

    bool get(int x, int y) const;
    void set(int x, int y, bool alive);
    void clear();
    std::size_t count() const;

    const std::vector<std::uint8_t>& bytes() const { return m_bytes; }
    BitGrid withBitOrder(BitOrder order) const;

    static std::size_t byteCount(Pair1<int> size);

private:
    std::size_t index(int x, int y) const;
    std::uint8_t mask(std::size_t bitInByte) const;

    int m_width;
    int m_height;
    BitOrder m_order;
    std::vector<std::uint8_t> m_bytes;
};
