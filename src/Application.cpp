#include "Application.h"

#include <chrono>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>
#include <thread>

#include "BoardSerializer.h"
#include "EndianInspector.h"
#include "Pair1.h"

Application::Application()
    : m_board(Pair1<int>(40, 20)),
      m_renderer(),
      m_library(),
      m_saveOrder(Endian::native())
{
}

void Application::printHelp() const
{
    std::cout << "Commands:\n"
              << "  help                     show this help\n"
              << "  show                     draw the board\n"
              << "  step [n]                 advance n generations\n"
              << "  run [n] [delayMs]        animate n generations\n"
              << "  clear                    clear the board\n"
              << "  random [density]         fill the board randomly\n"
              << "  patterns                 list available patterns\n"
              << "  place <name> <x> <y>     place a pattern\n"
              << "  size <w> <h>             create a new empty board\n"
              << "  rule <B3/S23>            change the rule\n"
              << "  wrap on|off              toggle toroidal edges\n"
              << "  bits                     dump packed bits\n"
              << "  flip                     switch LSB-first / MSB-first packing\n"
              << "  order little|big         byte order used for saving\n"
              << "  endian                   show endianness report\n"
              << "  save <file>              save the board\n"
              << "  load <file>              load the board\n"
              << "  quit                     exit\n";
}

void Application::show() const
{
    m_renderer.draw(m_board, std::cout);
}

void Application::animate(int steps, int delayMs)
{
    for (int i = 0; i < steps; ++i)
    {
        m_board.step();
        m_renderer.clearScreen(std::cout);
        show();
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }
}

void Application::resize(int width, int height)
{
    if (width < 1 || height < 1 || width > 200 || height > 200)
    {
        throw std::invalid_argument("Size must be between 1 and 200");
    }
    m_board = Board(Pair1<int>(width, height), m_board.rule(), m_board.wrap(), m_board.grid().bitOrder());
}

bool Application::execute(const std::string& line)
{
    std::istringstream input(line);
    std::string command;
    if (!(input >> command))
    {
        return true;
    }

    if (command == "quit" || command == "exit")
    {
        return false;
    }
    if (command == "help")
    {
        printHelp();
    }
    else if (command == "show")
    {
        show();
    }
    else if (command == "step")
    {
        int n = 1;
        input >> n;
        for (int i = 0; i < n; ++i)
        {
            m_board.step();
        }
        show();
    }
    else if (command == "run")
    {
        int n = 50;
        int delay = 100;
        input >> n >> delay;
        animate(n, delay);
    }
    else if (command == "clear")
    {
        m_board.clear();
        show();
    }
    else if (command == "random")
    {
        double density = 0.3;
        input >> density;
        std::random_device device;
        m_board.randomize(device(), density);
        show();
    }
    else if (command == "patterns")
    {
        for (const Pattern& p : m_library.all())
        {
            std::cout << "  " << p.name() << " (" << p.cells().size() << " cells)\n";
        }
    }
    else if (command == "place")
    {
        std::string name;
        int x = 0;
        int y = 0;
        if (!(input >> name >> x >> y))
        {
            std::cout << "Usage: place <name> <x> <y>\n";
            return true;
        }
        const Pattern* pattern = m_library.find(name);
        if (!pattern)
        {
            std::cout << "Unknown pattern: " << name << '\n';
            return true;
        }
        m_board.place(*pattern, x, y);
        show();
    }
    else if (command == "size")
    {
        int w = 0;
        int h = 0;
        if (!(input >> w >> h))
        {
            std::cout << "Usage: size <w> <h>\n";
            return true;
        }
        resize(w, h);
        show();
    }
    else if (command == "rule")
    {
        std::string notation;
        input >> notation;
        m_board.setRule(Rule(notation));
        std::cout << "Rule set to " << m_board.rule().notation() << '\n';
    }
    else if (command == "wrap")
    {
        std::string value;
        input >> value;
        if (value == "on")
        {
            m_board.setWrap(true);
        }
        else if (value == "off")
        {
            m_board.setWrap(false);
        }
        else
        {
            std::cout << "Usage: wrap on|off\n";
            return true;
        }
        std::cout << "Wrap is " << (m_board.wrap() ? "on" : "off") << '\n';
    }
    else if (command == "bits")
    {
        m_renderer.drawBits(m_board.grid(), std::cout);
    }
    else if (command == "flip")
    {
        const BitOrder next = m_board.grid().bitOrder() == BitOrder::LsbFirst
            ? BitOrder::MsbFirst
            : BitOrder::LsbFirst;
        m_board.setBitOrder(next);
        std::cout << "Bit order is now " << Endian::name(next) << '\n';
    }
    else if (command == "order")
    {
        std::string value;
        input >> value;
        if (value == "little")
        {
            m_saveOrder = ByteOrder::Little;
        }
        else if (value == "big")
        {
            m_saveOrder = ByteOrder::Big;
        }
        else
        {
            std::cout << "Usage: order little|big\n";
            return true;
        }
        std::cout << "Save byte order is " << Endian::name(m_saveOrder) << '\n';
    }
    else if (command == "endian")
    {
        EndianInspector::report(std::cout);
    }
    else if (command == "save")
    {
        std::string path;
        if (!(input >> path))
        {
            std::cout << "Usage: save <file>\n";
            return true;
        }
        BoardSerializer(m_saveOrder).save(m_board, path);
        std::cout << "Saved to " << path << " as " << Endian::name(m_saveOrder) << '\n';
    }
    else if (command == "load")
    {
        std::string path;
        if (!(input >> path))
        {
            std::cout << "Usage: load <file>\n";
            return true;
        }
        m_board = BoardSerializer::load(path);
        std::cout << "Loaded " << path << '\n';
        show();
    }
    else
    {
        std::cout << "Unknown command. Type 'help'.\n";
    }
    return true;
}

int Application::run()
{
    std::cout << "Conway's Game of Life\n";
    printHelp();
    show();

    std::string line;
    while (true)
    {
        std::cout << "> ";
        if (!std::getline(std::cin, line))
        {
            break;
        }
        try
        {
            if (!execute(line))
            {
                break;
            }
        }
        catch (const std::exception& e)
        {
            std::cout << "Error: " << e.what() << '\n';
        }
    }
    return 0;
}
