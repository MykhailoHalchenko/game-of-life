#include "EndianInspector.h"

#include <cstdint>
#include <cstring>
#include <iomanip>

#include "Endian.h"

namespace
{
void printBytes(std::ostream& out, std::uint32_t value)
{
    std::uint8_t raw[4];
    std::memcpy(raw, &value, 4);
    for (int i = 0; i < 4; ++i)
    {
        out << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(raw[i]) << ' ';
    }
    out << std::dec << std::setfill(' ');
}

void printBinary(std::ostream& out, std::uint8_t value)
{
    for (int bit = 7; bit >= 0; --bit)
    {
        out << ((value >> bit) & 1);
    }
}
}

void EndianInspector::report(std::ostream& out)
{
    const std::uint32_t sample = 0x12345678u;
    out << "Native byte order: " << Endian::name(Endian::native()) << '\n';
    out << "Sample value: 0x" << std::hex << sample << std::dec << '\n';

    out << "Memory bytes (native): ";
    printBytes(out, sample);
    out << '\n';

    out << "Memory bytes (little): ";
    printBytes(out, Endian::toOrder(sample, ByteOrder::Little));
    out << '\n';

    out << "Memory bytes (big):    ";
    printBytes(out, Endian::toOrder(sample, ByteOrder::Big));
    out << '\n';

    const std::uint8_t bits = 0x35;
    out << "Bits: ";
    printBinary(out, bits);
    out << " -> reversed: ";
    printBinary(out, Endian::reverseBits(bits));
    out << '\n';
}
