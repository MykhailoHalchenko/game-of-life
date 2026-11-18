#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <type_traits>

enum class ByteOrder : std::uint8_t
{
    Little = 0,
    Big = 1
};

enum class BitOrder : std::uint8_t
{
    LsbFirst = 0,
    MsbFirst = 1
};

class Endian
{
public:
    static ByteOrder native();
    static std::string name(ByteOrder order);
    static std::string name(BitOrder order);
    static std::uint8_t reverseBits(std::uint8_t value);

    template <typename T>
    static T swapBytes(T value)
    {
        static_assert(std::is_unsigned<T>::value, "Only unsigned integer types are supported");
        T result = 0;
        for (std::size_t i = 0; i < sizeof(T); ++i)
        {
            result = static_cast<T>((result << 8) | (value & 0xFF));
            value = static_cast<T>(value >> 8);
        }
        return result;
    }

    template <typename T>
    static T toOrder(T value, ByteOrder order)
    {
        return native() == order ? value : swapBytes(value);
    }

    template <typename T>
    static T fromOrder(T value, ByteOrder order)
    {
        return toOrder(value, order);
    }
};
