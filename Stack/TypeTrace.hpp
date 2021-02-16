#pragma once
#include <cstdio>



/*
\brief  Шаблонная структура для хранения информации о типах.
\detais Шаблонная структура для хранения информации о типах,
        в частности содежит функцию печати.
\note   При реализации функции печати следует в конце вывода
        вызвать fflush; НЕ использовать перевод строки.
*/
template<typename T>
struct TypeTrace
{
    static void print(const T& value)
    {
        printf("There isn't print function implementation for %s", typeid(T).name());
        fflush(stdout);
    }
};



//Несколько реализаций печати стандартных типов
void TypeTrace<char>::print(const char& value) { printf("%c", value); fflush(stdout); }
void TypeTrace<int>::print(const int& value) { printf("%d", value); fflush(stdout); }
void TypeTrace<unsigned int>::print(const unsigned int& value) { printf("%u", value); fflush(stdout); }