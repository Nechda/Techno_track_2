#pragma once
#include "Types.h"
#include "TypeTrace.hpp"
#include <iostream>


#define ON_CANARY 1

#if defined(ON_CANARY)
    #define STK_CANARY_PROTECTION(code) code
    const ui32 MAGIC_NUMBER = 0xAABBCCEEDD;
#else
    #define STK_CANARY_PROTECTION(code)
#endif


enum StackErrors
{
    STACK_OK,
    STACK_ATTACK_ON_LEFT_CANARY,
    STACK_ATTACK_ON_RIGHT_CANARY
};

/*
\brief –еализаци¤ стека на основе массива фиксированной длины
*/
template<typename T>
class Stack
{
    private:
        static const ui32 STACK_MAX_SIZE = 16;
        ui32 m_size;
        ui32 m_capacity;

    STK_CANARY_PROTECTION(
        public: ui32 CANARY_LEFT = MAGIC_NUMBER
    );
    private:
        T m_data[STACK_MAX_SIZE];
    STK_CANARY_PROTECTION(
        public: ui32 CANARY_RIGHT = MAGIC_NUMBER
    );
    public:
        Stack() :
            m_size(0),
            m_capacity(STACK_MAX_SIZE)
        {};
        ~Stack() {};
        void push(T value);
        void pop();
        T top();
        bool empty();
        void dump();
        StackErrors status();
};


template<typename T>
void Stack<T>::push(T value)
{
    if (m_size >= m_capacity)
        return;
    m_data[m_size] = value;
    m_size++;
}

template<typename T>
void Stack<T>::pop()
{
    if (!m_size)
        return;
    m_size--;
    m_data[m_size].~T();
}

template<typename T>
T Stack<T>::top()
{
    if (!m_size)
        return m_data[0];
    return m_data[m_size - 1];
}

template<typename T>
bool Stack<T>::empty()
{
    return !m_size;
}

template<typename T>
void Stack<T>::dump()
{
    for (ui32 i = 0; i < STACK_MAX_SIZE; i++)
    {
        printf("%c[%d]: ", i < m_size ? '*' : ' ', i);
        TypeTrace<T>::print(m_data[i]);
        printf("\n");
    }
}

template<typename T>
StackErrors Stack<T>::status()
{
    if (CANARY_LEFT != MAGIC_NUMBER)
        return STACK_ATTACK_ON_LEFT_CANARY;
    if (CANARY_RIGHT != MAGIC_NUMBER)
        return STACK_ATTACK_ON_RIGHT_CANARY;

    return STACK_OK;
}