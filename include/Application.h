#pragma once

#include <string>

#include "Board.h"
#include "ConsoleRenderer.h"
#include "Endian.h"
#include "Pattern.h"

class Application
{
public:
    Application();
    int run();

private:
    bool execute(const std::string& line);
    void printHelp() const;
    void show() const;
    void animate(int steps, int delayMs);
    void resize(int width, int height);

    Board m_board;
    ConsoleRenderer m_renderer;
    PatternLibrary m_library;
    ByteOrder m_saveOrder;
};
