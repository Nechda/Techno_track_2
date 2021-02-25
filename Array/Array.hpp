#pragma once
#include <stdexcept>
#include <climits>


#ifndef NDEBUG
    #include "Hash.h"
    #include <iostream>
    #define MAGIC_VALUE 0x8899AABB00225577
    #define DEBUG_CODE( code ) code
#else
    #define DEBUG_CODE( code ) 
#endif

enum class ARR_Error
{
    ARR_OK,
    ARR_HUGE_SIZE,
    ARR_ATTACK_ON_LEFT,
    ARR_ATTACK_ON_RIGHT,
    ARR_WRONG_HASH,
    ARR_NULLPTR_IN_THIS
};


template<typename T,size_t size_arr>
class Array
{
        DEBUG_CODE(friend void arr_test());
    private:
        DEBUG_CODE(uint64_t LEFT_CANARY = MAGIC_VALUE);
        T m_data[size_arr];
        DEBUG_CODE(uint64_t RIGHT_CANARY = MAGIC_VALUE);
        DEBUG_CODE(Hash m_hash);
        void statusCheker()
        {
            ARR_Error stat = status();
            if (stat == ARR_Error::ARR_OK)
                return;
            throw std::exception(errToStr(stat));
        }
    public:
        Array() : m_data()
        {
            DEBUG_CODE(updateHash());
        }
        Array(const Array<T, size_arr>& other)
        {
            for (size_t i = 0; i < size_arr; i++)
                m_data[i] = T(other.m_data[i]);

            DEBUG_CODE(updateHash());
        }
        Array<T, size_arr>& operator = (const Array<T, size_arr>& other)
        {
            for (size_t i = 0; i < size_arr; i++)
                m_data[i] = T(other.m_data[i]);
            DEBUG_CODE(updateHash());
            return *this;
        }
        ~Array() {};
        size_t size()
        {
            return size_arr;
        }
        T& at(size_t index)
        {
            statusCheker();
            if (size_arr <= index)
                throw std::out_of_range("Out of range. Index is: " + std::to_string(index));
            return m_data[index];
        }
        T& operator [](size_t index)
        {
            statusCheker();
            return m_data[index];
        }

        static const char* errToStr(ARR_Error e)
        {
            switch (e)
            {
                case ARR_Error::ARR_OK:
                    return "Ok";
                case ARR_Error::ARR_HUGE_SIZE:
                    return "Huge size";
                case ARR_Error::ARR_ATTACK_ON_LEFT:
                    return "Attack on left";
                case ARR_Error::ARR_ATTACK_ON_RIGHT:
                    return "Attack on right";
                case ARR_Error::ARR_WRONG_HASH:
                    return "Wrong hash";
                default:
                    return "Undefined error";
            }
        }
         
        ARR_Error status()
        {
            #ifndef NDEBUG
                if (this == nullptr)
                    return ARR_Error::ARR_NULLPTR_IN_THIS;
                if ( size_arr > INT_MAX)
                    return ARR_Error::ARR_HUGE_SIZE;
                if (LEFT_CANARY != MAGIC_VALUE)
                    return ARR_Error::ARR_ATTACK_ON_LEFT;
                if (RIGHT_CANARY != MAGIC_VALUE)
                    return ARR_Error::ARR_ATTACK_ON_RIGHT;
                Hash h = getHash(m_data, sizeof(T) * size_arr);
                if (m_hash != h)
                    return ARR_Error::ARR_WRONG_HASH;
                return ARR_Error::ARR_OK;
            #else
                return ARR_Error::ARR_OK;
            #endif
        }

        #ifndef NDEBUG
        void dump()
        {
            printf("~~~Dumping~~~\n");
            printf("Array<%s,%d>{\n", typeid(T).name(), size_arr);
            printf("    Magic  number  is = 0x%016llX\n", MAGIC_VALUE);
            printf("        Left   canary = 0x%016llX\n", LEFT_CANARY);
            printf("       Right   canary = 0x%016llX\n", RIGHT_CANARY);
            printf("    original   m_hash = 0x%016llX\n", m_hash);
            Hash h = getHash(m_data, sizeof(T) * size_arr);
            printf("    recalculated hash = 0x%016llX\n", h);
            printf("    Data{\n");
            for (size_t i = 0; i < size_arr; i++)
                std::cout << "        array[" << i << "] = "<< m_data[i] << std::endl;
            printf("    }\n");
            printf("}\n");
            printf("~~~End~~~\n");
        }

        void updateHash()
        {
            m_hash = getHash(m_data, sizeof(T) * size_arr);
        }
        #endif
};