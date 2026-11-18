#include "Endian.h"

#include <cstring>

ByteOrder Endian::native()
{
    const std::uint16_t probe = 1;
    std::uint8_t first = 0;
    std::memcpy(&first, &probe, 1);
    return first == 1 ? ByteOrder::Little : ByteOrder::Big;
}

std::string Endian::name(ByteOrder order)
{
    return order == ByteOrder::Little ? "little-endian" : "big-endian";
}

std::string Endian::name(BitOrder order)
{
    return order == BitOrder::LsbFirst ? "LSB-first" : "MSB-first";
}

std::uint8_t Endian::reverseBits(std::uint8_t value)
{
    std::uint8_t result = 0;
    for (int i = 0; i < 8; ++i)
    {
        result = static_cast<std::uint8_t>((result << 1) | (value & 1));
        value = static_cast<std::uint8_t>(value >> 1);
    }
    return result;
}
