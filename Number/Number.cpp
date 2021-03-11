#include "Number.h"
#include "Tools/Console.h"

using namespace std;
using namespace dot;

i64 Number::nOpration = 0;
i64 Number::nObjects = 0;
i64 Number::nAliveObjects = 0;
i64 Number::nOparationsLeft = 2; /// ограничение на количество операций приведения типа 

/**
    \brief Функция печатает инфу об операндах
    \param [in]  lhs  Константная ссылка на левый операнд
    \param [in]  rhs  Константная ссылка на правый операнд
    \param [in]  str  Строка, содержащая название операции
*/
static inline void print_info_about_operator(
    const Number& lhs,
    const Number& rhs,
    const char* str
)
{
    cout << YELLOW << Margin(StackTracer::stack_level);
    printf("operator %s (Number& = [%p], Number& = [%p])\n", str, &lhs, &rhs);
    $_
        cout << LIGHTMAGENTA << Margin(StackTracer::stack_level);
        printf("|-->");
        lhs.dump();
        cout << LIGHTMAGENTA << Margin(StackTracer::stack_level);
        printf("+-->");
        rhs.dump();
    $$_
}

/**
    \brief Функция проверяет структуру на ошибки
    \note  Данная функция генерит исключение, если
           указатель this == nullptr
*/
void Number::checkStatus()
{
    if (this == nullptr)
        throw std::exception("this contain nullptr!");
    if (m_isOverflow)
        throw std::exception("overflowing!\n");
}

/**
    \brief Функция выводит полную информацию о структуре на экран
*/
void Number::dump() const
{
    printf("[%p] : { m_number = %ld }\n", this, !this ? 0 : m_number);
}


/**
    \brief Конструктор копирования
*/
Number::Number(const Number& other, ADDITION_PARAM(std::string name))
{$ $PRNT
    m_name = name;
    m_id = nObjects++;
    checkStatus();
    m_number = other.m_number;
    ADDITION_PRINT(
        createNode(*this,"copy ctor()");
        createLine(other, *this);
        cout << GREEN << Margin(StackTracer::stack_level);
        printf("ctor(Number& = [%p]) this=", &other);
        dump();
        $_
            cout << LIGHTMAGENTA << Margin(StackTracer::stack_level);
            printf("+-->");
            other.dump();
        $$_
    )
}

/**
    \brief Оператор присваивания
*/
Number& Number::operator = (const Number& other)
{$ $PRNT
    checkStatus();
    if(this == &other)
        return *this;
    m_number = other.m_number;
    ADDITION_PRINT(
        m_nStage++;
        createNode(*this);

        createOperator(*this, other, "operator =");
        cout << CYAN << Margin(StackTracer::stack_level);
        printf("operator = (Number& = [%p]) this=", &other);
        dump();
        $_
            cout << LIGHTMAGENTA << Margin(StackTracer::stack_level);
            printf("+-->");
            other.dump();
        $$_
    )
    return *this;
}

/**
    \brief Оператор постфиксного инкремента
*/
Number Number::operator ++ (int)
{$ $PRNT
    checkStatus();
    ADDITION_PRINT(
        cout << YELLOW << Margin(StackTracer::stack_level);
        printf("operator ++ (int) this=");
        dump();
    )
    m_isOverflow |= std::numeric_limits<i64>::max() == m_number;
    checkStatus();
    m_number++;
    return Number(m_number - 1);
}

/**
    \brief Оператор префиксного инкремента
*/
Number Number::operator ++ ()
{$ $PRNT
    checkStatus();
    ADDITION_PRINT(
        cout << YELLOW << Margin(StackTracer::stack_level);
        printf("operator ++ () this=");
        dump();
    )
    m_isOverflow |= std::numeric_limits<i64>::max() == m_number;
    checkStatus();
    m_number++;
    return *this;
}


/**
    \brief Оператор постфиксного декремента
*/
Number Number::operator -- (int)
{$ $PRNT
    checkStatus();
    ADDITION_PRINT(
        cout << YELLOW << Margin(StackTracer::stack_level);
        printf("operator -- (int) this=");
        dump();
    )
    m_isOverflow |= std::numeric_limits<i64>::min() == m_number;
    checkStatus();
    m_number--;
    return Number(m_number + 1);
}

/**
    \brief Оператор префиксного декремента
*/
Number Number::operator -- ()
{$ $PRNT
    checkStatus();
    ADDITION_PRINT(
        cout << YELLOW << Margin(StackTracer::stack_level);
        printf("operator -- () this=");
        dump();
    )
    m_isOverflow |= std::numeric_limits<i64>::min() == m_number;
    checkStatus();
    m_number--;
    return *this;
}

/**
    \brief Переопределение оператора << для потока вывода
*/
std::ostream& operator << (std::ostream& out, const Number& num)
{$
    if(&num)
        out << num.m_number;
    else
        out << "{nullptr}";
    return out;
}

/**
    \brief Переопределение оператора +
    \param [in] lhs  Константная ссылка на левый операнд
    \param [in] rhs  Константная ссылка на правый операнд
    \retur Объект Number, который является суммой lhs и rhs
    \note  В случае переполнения типа i64 бросается исключение.
           Если одна из ссылкок была на нулевой указатель, то
           тоже бросается исключение.
*/
Number operator + (const Number& lhs, const Number& rhs)
{$ $PRNT
    ADDITION_PRINT(
        createOperator(lhs, rhs, "operator +");
        print_info_about_operator(lhs, rhs, "+");
    )

    if(!&lhs || !&rhs)
        throw std::exception("this contain nullptr!");

    bool isOverflow =
           lhs.m_number > 0 && rhs.m_number > std::numeric_limits<i64>::max() - lhs.m_number
        || rhs.m_number < 0 && lhs.m_number < std::numeric_limits<i64>::min() - rhs.m_number;
    
    if(isOverflow)
        throw std::exception("operator + lead to overflowing!");

    return Number(lhs.m_number + rhs.m_number);
}

/**
    \brief Переопределение оператора -
    \param [in] lhs  Константная ссылка на левый операнд
    \param [in] rhs  Константная ссылка на правый операнд
    \retur Объект Number, который является разностью lhs и rhs
    \note  В случае переполнения типа i64 бросается исключение.
           Если одна из ссылкок была на нулевой указатель, то
           тоже бросается исключение.
*/
Number operator - (const Number& lhs, const Number& rhs)
{$ $PRNT
    ADDITION_PRINT(
        createOperator(lhs, rhs, "operator -");
        print_info_about_operator(lhs, rhs, "-");
    )

    if(!&lhs || !&rhs)
        throw std::exception("this contain nullptr!");

    bool isOverflow =
           lhs.m_number < 0 && -lhs.m_number + std::numeric_limits<i64>::min() > -rhs.m_number
        || lhs.m_number > 0 && lhs.m_number - std::numeric_limits<i64>::max() > rhs.m_number;
    
    if(isOverflow)
        throw std::exception("operator - lead to overflowing!");

    return Number(lhs.m_number - rhs.m_number);
}

/**
    \brief Переопределение оператора *
    \param [in] lhs  Константная ссылка на левый операнд
    \param [in] rhs  Константная ссылка на правый операнд
    \retur Объект Number, который является произведением lhs и rhs
    \note  В случае переполнения типа i64 бросается исключение.
           Если одна из ссылкок была на нулевой указатель, то
           тоже бросается исключение.
*/
Number operator * (const Number& lhs, const Number& rhs)
{$ $PRNT
    ADDITION_PRINT(
        createOperator(lhs, rhs, "operator *");
        print_info_about_operator(lhs, rhs, "*");
    )

    if(!&lhs || !&rhs)
        throw std::exception("this contain nullptr!");

    bool isOverflow =
           lhs.m_number > 0 && rhs.m_number > std::numeric_limits<i64>::max() / lhs.m_number
        || lhs.m_number < 0 && rhs.m_number < std::numeric_limits<i64>::min() / lhs.m_number;
    
    if(isOverflow)
        throw std::exception("operator * lead to overflowing!");

    return Number(lhs.m_number * rhs.m_number);
}

/**
    \brief Переопределение оператора /
    \param [in] lhs  Константная ссылка на левый операнд
    \param [in] rhs  Константная ссылка на правый операнд
    \retur Объект Number, который является суммой lhs и rhs
    \note  В случае деления на ноль бросается исключение.
           Если одна из ссылкок была на нулевой указатель, то
           тоже бросается исключение.
*/
Number operator / (const Number& lhs, const Number& rhs)
{$ $PRNT
    ADDITION_PRINT(
        createOperator(lhs, rhs, "operator /");
        print_info_about_operator(lhs, rhs, "/");
    )

    if(!&lhs || !&rhs)
        throw std::exception("this contain nullptr!");

    if (rhs.m_number == 0)
        throw std::exception("division by zero!");

    return Number(lhs.m_number / rhs.m_number);
}

/**
    \brief Переопределение операторов стравнения
    \param [in] lhs  Константная ссылка на левый операнд
    \param [in] rhs  Константная ссылка на правый операнд
    \note  Если одна из ссылкок была на нулевой указатель, то
           бросается исключение.
*/
///@{
bool operator == (const Number& lhs, const Number& rhs)
{
    ADDITION_PRINT(
        print_info_about_operator(lhs, rhs, "==");
    )
    if(!&lhs || !&rhs)
        throw std::exception("this contain nullptr!");
    return lhs.m_number == rhs.m_number; 
}
bool operator != (const Number& lhs, const Number& rhs)
{
    ADDITION_PRINT(
        print_info_about_operator(lhs, rhs, "!=");
    )
    if(!&lhs || !&rhs)
        throw std::exception("this contain nullptr!");
    return lhs.m_number != rhs.m_number; 
}
bool operator < (const Number& lhs, const Number& rhs)
{
    ADDITION_PRINT(
        print_info_about_operator(lhs, rhs, "<");
    )
        if (!&lhs || !&rhs)
            throw std::exception("this contain nullptr!");
    return lhs.m_number < rhs.m_number;
}
bool operator > (const Number& lhs, const Number& rhs) 
{
    ADDITION_PRINT(
        print_info_about_operator(lhs, rhs, ">");
    )
        if (!&lhs || !&rhs)
            throw std::exception("this contain nullptr!");
    return lhs.m_number > rhs.m_number;
}
bool operator <= (const Number& lhs, const Number& rhs)
{
    ADDITION_PRINT(
        print_info_about_operator(lhs, rhs, "<=");
    )
        if (!&lhs || !&rhs)
            throw std::exception("this contain nullptr!");
    return lhs.m_number <= rhs.m_number;
}
bool operator >= (const Number& lhs, const Number& rhs)
{
    ADDITION_PRINT(
        print_info_about_operator(lhs, rhs, ">=");
    )
        if (!&lhs || !&rhs)
            throw std::exception("this contain nullptr!");
    return lhs.m_number >= rhs.m_number;
}
///@}