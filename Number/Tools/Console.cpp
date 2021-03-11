#include "Console.h"

#if WIN32
    #include <Windows.h>

    static void setColorAndBackground(int ForgC, int BackC = 0)
    {
        WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
    }
#else
    static void setColorAndBackground(int ForgC, int BackC = 0) {}
#endif

std::ostream& operator<<(std::ostream& os, const ConsoleColour &c)
{
    setColorAndBackground(c);
    return os;
}

std::ostream& operator<<(std::ostream& os, const Margin& m)
{
    printf("%*s", m.m_level << 2, "");
    fflush(stdout);
    return os;
}