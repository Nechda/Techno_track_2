#pragma once
#include <iostream>

/// \brief энам для описания цветов консоли
enum ConsoleColour
{
    BLACK, BLUE, GREEN, CYAN,
    RED, MAGENTA, BROWN, LIGHTGRAY,
    DARKGRAY, LIGHTBLUE, LIGHTGREEN,
    LIGHTCYAN, LIGHTRED, LIGHTMAGENTA,
    YELLOW, WHITE
};

/// \brief структура для реализации отспупов в консоли
struct Margin
{
    public:
        int m_level = 0;
        Margin(int level) : m_level(level) {};
    private:
        Margin() = delete;
};

/// \brief операторы << для потока вывода, регулирующие число отступов и цвет текста
std::ostream& operator<<(std::ostream& os, const ConsoleColour &c);
std::ostream& operator<<(std::ostream& os, const Margin& m);