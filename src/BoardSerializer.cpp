#include "BoardSerializer.h"

#include <cstdint>
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <vector>

namespace
{
const char kMagic[4] = {'G', 'O', 'L', '1'};
const std::uint32_t kMaxDimension = 4096;

template <typename T>
void writeValue(std::ostream& out, T value, ByteOrder order)
{
    const T converted = Endian::toOrder(value, order);
    char raw[sizeof(T)];
    std::memcpy(raw, &converted, sizeof(T));
    out.write(raw, sizeof(T));
}

template <typename T>
T readValue(std::istream& in, ByteOrder order)
{
    char raw[sizeof(T)];
    in.read(raw, sizeof(T));
    if (!in)
    {
        throw std::runtime_error("Unexpected end of file");
    }
    T value;
    std::memcpy(&value, raw, sizeof(T));
    return Endian::fromOrder(value, order);
}

std::uint8_t readByte(std::istream& in)
{
    char c = 0;
    in.read(&c, 1);
    if (!in)
    {
        throw std::runtime_error("Unexpected end of file");
    }
    return static_cast<std::uint8_t>(c);
}

void writeByte(std::ostream& out, std::uint8_t value)
{
    out.write(reinterpret_cast<const char*>(&value), 1);
}
}

BoardSerializer::BoardSerializer(ByteOrder order)
    : m_order(order)
{
}

void BoardSerializer::save(const Board& board, const std::string& path) const
{
    std::ofstream out(path, std::ios::binary);
    if (!out)
    {
        throw std::runtime_error("Cannot open file for writing: " + path);
    }

    const BitGrid& grid = board.grid();

    out.write(kMagic, 4);
    writeByte(out, static_cast<std::uint8_t>(m_order));
    writeByte(out, static_cast<std::uint8_t>(grid.bitOrder()));
    writeByte(out, board.wrap() ? 1 : 0);
    writeByte(out, 0);

    writeValue<std::uint32_t>(out, static_cast<std::uint32_t>(grid.width()), m_order);
    writeValue<std::uint32_t>(out, static_cast<std::uint32_t>(grid.height()), m_order);
    writeValue<std::uint32_t>(out, static_cast<std::uint32_t>(board.generation()), m_order);
    writeValue<std::uint16_t>(out, board.rule().birthMask(), m_order);
    writeValue<std::uint16_t>(out, board.rule().surviveMask(), m_order);

    const std::vector<std::uint8_t>& bytes = grid.bytes();
    for (std::size_t i = 0; i < bytes.size(); i += 4)
    {
        std::uint32_t word = 0;
        for (std::size_t k = 0; k < 4 && i + k < bytes.size(); ++k)
        {
            word |= static_cast<std::uint32_t>(bytes[i + k]) << (8 * k);
        }
        writeValue<std::uint32_t>(out, word, m_order);
    }

    if (!out)
    {
        throw std::runtime_error("Failed to write file: " + path);
    }
}

Board BoardSerializer::load(const std::string& path)
{
    std::ifstream in(path, std::ios::binary);
    if (!in)
    {
        throw std::runtime_error("Cannot open file for reading: " + path);
    }

    char magic[4];
    in.read(magic, 4);
    if (!in || std::memcmp(magic, kMagic, 4) != 0)
    {
        throw std::runtime_error("Invalid file format");
    }

    const std::uint8_t orderFlag = readByte(in);
    const std::uint8_t bitFlag = readByte(in);
    const std::uint8_t wrapFlag = readByte(in);
    readByte(in);

    if (orderFlag > 1 || bitFlag > 1)
    {
        throw std::runtime_error("Corrupted header");
    }

    const ByteOrder order = static_cast<ByteOrder>(orderFlag);
    const BitOrder bitOrder = static_cast<BitOrder>(bitFlag);

    const std::uint32_t width = readValue<std::uint32_t>(in, order);
    const std::uint32_t height = readValue<std::uint32_t>(in, order);
    const std::uint32_t generation = readValue<std::uint32_t>(in, order);
    const std::uint16_t birth = readValue<std::uint16_t>(in, order);
    const std::uint16_t survive = readValue<std::uint16_t>(in, order);

    if (width == 0 || height == 0 || width > kMaxDimension || height > kMaxDimension)
    {
        throw std::runtime_error("Invalid grid dimensions");
    }

    const Pair1<int> size(static_cast<int>(width), static_cast<int>(height));
    const std::size_t total = BitGrid::byteCount(size);
    std::vector<std::uint8_t> bytes;
    bytes.reserve(total + 3);

    const std::size_t words = (total + 3) / 4;
    for (std::size_t i = 0; i < words; ++i)
    {
        const std::uint32_t word = readValue<std::uint32_t>(in, order);
        for (std::size_t k = 0; k < 4; ++k)
        {
            bytes.push_back(static_cast<std::uint8_t>((word >> (8 * k)) & 0xFF));
        }
    }
    bytes.resize(total);

    return Board(BitGrid(size, bitOrder, std::move(bytes)), Rule(birth, survive), wrapFlag != 0, generation);
}
