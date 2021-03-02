#pragma once

#include "Tools/Types.h"
#include "Tools/Console.h"
#include "Tools/StackTracerMin.h"

//#define PRINT_CONSTRUCTORS_INFO
    
#ifdef PRINT_CONSTRUCTORS_INFO
    #define ADDITION_PRINT( code ) code
#else
    #define ADDITION_PRINT( code ) 
#endif

/**
    \brief  Класс реализует целое число
*/
class Number
{
    private:
        static i64 nOparationsLeft; ///< Ограничитель на количество операций приведения типа
        i64 m_number;               ///< Поле для хранения самого числа
        static i64 nAliveObjects;   ///< Поле для хранения количества живых объекто
        bool isOverflow = false;    ///< Флаг для переполнения
        void checkStatus();         ///< функция проверяет структуру на ошибки и в случае проблем кидает исключение
    public:
        /// конструктор по-умолчанию
        /// Number num = Number();
        /// Number num;
        Number() : m_number()
        {$
            nAliveObjects++;
            ADDITION_PRINT(
                std::cout << GREEN << Margin(__stack_level);
                printf("~~~>ctor() this=");
                dump();
            );
        };

        ~Number()
        {$
            nAliveObjects--;
            ADDITION_PRINT(
                std::cout << LIGHTRED << Margin(__stack_level);
                printf("~~~>dtor() this=");
                dump();
                std::cout << WHITE;
            )
        }

        /// еще один конструктор
        /// Number num = Number(10);
        /// Number num = 10;
        Number(i64 number) : m_number(number)
        {$
            nAliveObjects++;
            ADDITION_PRINT(
                std::cout << GREEN << Margin(__stack_level);
                printf("~~~>ctor(%ld) this=", number);
                dump();
            )
        }

        /// \brief Функция возвращает количество созданных объектов
        static ui64 getCountOfAlive() { return nAliveObjects; }
        void dump() const;
        Number(const Number& other);
        Number& operator = (const Number& other);
        Number operator ++ (int);
        Number operator ++ ();
        Number operator -- (int);
        Number operator -- ();
        friend Number operator + (const Number& lhs, const Number& rhs);
        friend Number operator - (const Number& lhs, const Number& rhs);
        friend Number operator * (const Number& lhs, const Number& rhs);
        friend Number operator / (const Number& lhs, const Number& rhs);

        friend bool operator == (const Number& lhs, const Number& rhs);
        friend bool operator != (const Number& lhs, const Number& rhs);
        friend bool operator <  (const Number& lhs, const Number& rhs);
        friend bool operator >  (const Number& lhs, const Number& rhs);
        friend bool operator <= (const Number& lhs, const Number& rhs);
        friend bool operator >= (const Number& lhs, const Number& rhs);

        explicit operator i64()
        {
            if (!nOparationsLeft--)
                throw std::exception(
                    "Hey, you quite often cast Number into i64! "
                    "May be it will be better if you'll start "
                    "use standard types?");
            return m_number;
        }

        friend std::ostream& operator << (std::ostream& out, const Number& num);
};