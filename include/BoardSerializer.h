#pragma once

#include <string>

#include "Board.h"
#include "Endian.h"

class BoardSerializer
{
public:
    explicit BoardSerializer(ByteOrder order);

    void save(const Board& board, const std::string& path) const;
    static Board load(const std::string& path);

private:
    ByteOrder m_order;
};
