#pragma once

#include "Tools/Types.h"
#include "Tools/Console.h"
#include "Tools/StackTracerMin.h"
#include "Dot.h"
#include <sstream>

//#define PRINT_CONSTRUCTORS_INFO

// история объектов
// построение графа 
//     стрелки --- тип операции над объектом
//     узлы    --- объекты класса
// объединение узлов в группы (dot)
// оператор range в dot
// task: получить всевозможные графы
// thin line --- отображение стрелочек между последовательными действиями
// над объектом (для фиксирования порядка выполнения) 
#define PRINT_CONSTRUCTORS_INFO
#ifdef PRINT_CONSTRUCTORS_INFO
    #define ADDITION_PRINT( code ) code
    #define ADDITION_PARAM( code ) code
#else
    #define ADDITION_PRINT( code ) 
    #define ADDITION_PARAM( code )
#endif


/**
    \brief  Класс реализует целое число
*/
class Number
{
        friend void dot::createNode(const Number& num, const std::string& func);
        friend void dot::createLine(const std::string& src, const Number& dst, std::string label, std::string style);
        friend void dot::createLine(const Number& src, const Number& dst, std::string label, std::string style);
        friend void dot::createOperator(const Number& lhs, const Number& rhs, std::string operatorStr);
        friend void dot::createHistory(const Number& num);
    private:
        unsigned m_id; //внтуренний номер объекта
        unsigned m_nStage = 0; //внутренния номер состояния объекта
        std::string m_name;

        static i64 nObjects;
        static i64 nOparationsLeft; ///< Ограничитель на количество операций приведения типа
        i64 m_number;               ///< Поле для хранения самого числа
        static i64 nAliveObjects;   ///< Поле для хранения количества живых объекто
        bool m_isOverflow = false;  ///< Флаг для переполнения
        void checkStatus();         ///< функция проверяет структуру на ошибки и в случае проблем кидает исключение
        std::string m_history;
    public:

        void setName(std::string name) { m_name = name; };

        /// конструктор по-умолчанию
        /// Number num = Number();
        /// Number num;
        Number(ADDITION_PARAM(std::string name = "")) : m_number()
        {$
            m_name = name;
            m_id = nObjects++;
            nAliveObjects++;
            ADDITION_PRINT(
                dot::createNode(*this, "ctor()");
                std::cout << GREEN << MARGIN;
                printf("ctor() this=");
                dump();
            );
        };

        ~Number()
        {$
            nAliveObjects--;
            m_nStage++;
            ADDITION_PRINT(
                dot::createNode(*this, "dtor()");
                dot::createHistory(*this);
                std::cout << LIGHTRED << MARGIN;
                printf("dtor() this=");
                dump();
                std::cout << WHITE;
            )
        }

        /// еще один конструктор
        /// Number num = Number(10);
        /// Number num = 10;
        Number(i64 number, ADDITION_PARAM(std::string name = "")) : m_number(number)
        {$
            m_name = name;
            m_id = nObjects++;
            nAliveObjects++;
            ADDITION_PRINT(
                dot::createNode(*this,"ctor(" + std::to_string(number) + ")");
                std::cout << GREEN << MARGIN;
                printf("ctor(%ld) this=", number);
                dump();
            )
        }

        /// \brief Функция возвращает количество созданных объектов
        static ui64 getCountOfAlive() { return nAliveObjects; }
        void dump() const;
        Number(const Number& other, ADDITION_PARAM(std::string name = ""));
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
        static i64 nOpration;
};

