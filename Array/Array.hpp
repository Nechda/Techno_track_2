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

/**
\brief  Описание возможных состояний массива
*/
enum class ARR_Error
{
    ARR_OK,
    ARR_HUGE_SIZE,
    ARR_ATTACK_ON_LEFT,
    ARR_ATTACK_ON_RIGHT,
    ARR_WRONG_HASH,
    ARR_NULLPTR_IN_THIS
};

/**
\brief  Шаблонная реализация массива фиксированной длины
*/
template<typename T,size_t size_arr>
class Array
{
        DEBUG_CODE(friend void arr_test());
    private:
        DEBUG_CODE(uint64_t LEFT_CANARY = MAGIC_VALUE);
        T m_data[size_arr];
        DEBUG_CODE(uint64_t RIGHT_CANARY = MAGIC_VALUE);
        DEBUG_CODE(Hash m_hash);

        /**
            \brief   Функция производит проверку целостности стркутуры
            \details Функция проиводит проверку целостности структуры
                     и в случае нарушения целостности кидает исключение
                     с информацией об ошибке.
        */
        void statusCheker()
        {
            ARR_Error stat = status();
            if (stat == ARR_Error::ARR_OK)
                return;
            throw std::exception(errToStr(stat));
        }
    public:
        
        ///Конструктор по-умолчанию 
        Array() : m_data()
        {
            DEBUG_CODE(updateHash());
        }

        ///Конструктор копирования
        Array(const Array<T, size_arr>& other)
        {
            for (size_t i = 0; i < size_arr; i++)
                m_data[i] = T(other.m_data[i]);

            DEBUG_CODE(updateHash());
        }

        ///Оператор присваивания
        Array<T, size_arr>& operator = (const Array<T, size_arr>& other)
        {
            for (size_t i = 0; i < size_arr; i++)
                m_data[i] = T(other.m_data[i]);
            DEBUG_CODE(updateHash());
            return *this;
        }
        ~Array() {};


        /**
            \breif    Метод возвращает размер массива
            \return   Размер массива
        */
        size_t size() const {return size_arr;}


        /**
            \brief   Метод обращается к элементу массива с проверкой
                     выхода за границы
            \param   [in]  index  Индекс элемента в массиве
            \return  Возвращает ссылку на элемент массива
            \note    Если происходит выход за границы, то бросается
                     std::out_of_range. В случае повреждения структуры
                     бросается std::exception с информацией об ошибке.
        */
        T& at(size_t index)
        {
            statusCheker();
            if (size_arr <= index)
                throw std::out_of_range("Out of range. Index is: " + std::to_string(index));
            return m_data[index];
        }


        /**
            \brief   Оператор []
            \param   [in]  index  Индекс элемента в массиве
            \return  Возвращает ссылку на элемент массива
            \note    Данный метод не проверяет выход за пределы массива.
                     В случае повреждения структуры бросается 
                     std::exception с информацией об ошибке.
        */
        T& operator [](size_t index)
        {
            statusCheker();
            return m_data[index];
        }


        /**
            \brief   Функция генерит поясняющую строку к коду ошибки
            \param   [in]  e   Код ошибки, который требуется расшифровать
            \return  Поясняющая строка
        */
        static const char* errToStr(ARR_Error e) const
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
        

        /**
            \brief   Функция проверяет целостность массива
            \return  Код ошибки, если структура повреждена
            \note    Если структура целая, то возвращается
                     ARR_Error::ARR_OK;
        */
        ARR_Error status() const
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


        /**
            \brief   Функция печати полной информации о структуре
                     на экран
            \note    Требуется наличие преопределенного оператора
                     вывода.
        */
        void dump() const
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


        /**
            \brief   Функция пересчета хеша для данных
        */
        void updateHash()
        {
            m_hash = getHash(m_data, sizeof(T) * size_arr);
        }
        #endif
};