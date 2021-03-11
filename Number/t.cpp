#include "Tools/Types.h"
#include "Tools/ChainException.h"
#include "Tools/Console.h"
#include "Number.h"
#include <time.h>

using namespace std;


/**
    \brief   Кастомный рандом
    \return  Возвращает случаеное число
             в диапазоне [0; 4294967295]
*/
static i64 random()
{
    return rand() & 0xFFFF'FFFF;
}

/**
    \brief  Сортировка пузырьком на массиве из Number
*/
static void t_sorting()
{
    const i32 ARRAY_SIZE = 8;
    Number arr[ARRAY_SIZE];
    for (i32 i = 0; i < ARRAY_SIZE; i++)
        arr[i] = random();

    Number tmp;
    Number max_i = Number(ARRAY_SIZE - 1);
    for(Number i = 0; i < max_i; i++)
        for(Number j = i + 1; j < Number(ARRAY_SIZE); j++)
            if (arr[i64(i)] > arr[i64(j)])
            {
                tmp = arr[i64(i)];
                arr[i64(i)] = arr[i64(j)];
                arr[i64(j)] = tmp;
            }

    bool flag = 0;
    for (Number i = 0; i < max_i && !flag; i++)
        if (arr[i64(i)] > arr[i64(i) + 1])
            flag |= 1;

    if (flag)
    {
        cout << RED << "Test failed!\n" << WHITE;
        for (i32 i = 0; i < ARRAY_SIZE; i++)
            printf("%lld ", i64(arr[i]));
    }
    else
        cout << GREEN << "Ok " << WHITE;
}

/**
    \brief  Функция вычисляет n - тое число Фибоначчи
    \detail Функция вычисляет n - тое число Фибоначчи,
            используя в качестве числа класс Number
    \param  [in]  n  Номер числа Фибоначчи
    \return Объект типа Number, хранящий n - тое число 
            Фибоначчи
    \note   Нулевому числу отвечает 0, если передается 
            отрицательный индекс, то возвращается 0.
*/
static Number t_fib_Number(i64 n)
{
    if (n < 0)
        return 0;
    bool pingpong = 0;
    Number num[2] = { 0, 1 };
    while (n--)
    {
        num[pingpong ^ 1] = num[pingpong] + num[pingpong ^ 1];
        pingpong ^= 1;
    }
    return num[pingpong ^ 1];
}


/**
    \brief  Функция вычисляет n - тое число Фибоначчи
    \deltai Функция вычисляет n - тое число Фибоначчи,
            используя в качестве числа i64
    \param  [in]  n  Номер числа Фибоначчи
    \return Число типа i64, хранящее n - тое число
            Фибоначчи
    \note   Нулевому числу отвечает 0, если передается
            отрицательный индекс, то возвращается 0.
*/
static i64 t_fib_i64(i64 n)
{
    if (n < 0)
        return 0;
    bool pingpong = 0;
    i64 num[2] = { 0, 1 };
    while (n--)
    {
        num[pingpong ^ 1] = num[pingpong] + num[pingpong ^ 1];
        pingpong ^= 1;
    }
    return num[pingpong ^ 1];
}

/**
    \brief   Функция проверяет, нужное ли выпало исключение 
    \detail  Функция проверяет, нужное ли выпало исключение,
             если нет, то генерится информация о объектах
             и пишется причина, что именно ожидалось.
    \param   [in]  e    std::exception, которое было перехвачено
    \param   [in]  a    Первое число, участвовавшее в тесте
    \param   [in]  b    Второе число, участвовавшее в тесте
    \param   [in]  msg  Текст исключения, которое мы ожидали
*/
static void printTestExeptionInfo(
    const std::exception& e,
    const Number& a,
    const Number& b,
    const char* expectedMsg
)
{
    if (strcmp(e.what(), expectedMsg))
    {
        cout << RED << "Test failed!\n" << WHITE
             << "Cathed unexpected exception: "
             << e.what() << endl;
        a.dump();
        b.dump();
        printf("Should be overflowed");
    }
    else
        cout << GREEN << "Ok " << WHITE;
}

/**
    \brief  Реализация теста, проверяющий переполнение чисел
*/
void t_overflowing()
{
    const i64 MAX = std::numeric_limits<i64>::max();
    const i64 MIN = std::numeric_limits<i64>::min() + 1;

    const ui32 NTEST = 512;

    printf("Summation:\n");
    for (ui32 i = 0; i < NTEST; i++)
    {
        Number a = Number(MAX / 2 + (random() & 0xFFFF) + 1);
        Number b = Number(MAX / 2 + (random() & 0xFFFF) + 1);
        try
        {
            a + b;
            cout << RED << "Test failed!\n" << WHITE;
            a.dump();
            b.dump();
            printf("Should be overflowed");
        }
        catch (std::exception& e)
        {
            printTestExeptionInfo(e, a, b, "operator + lead to overflowing!");
        }
    }
    printf("\n");


    printf("Subtraction:\n");
    for (ui32 i = 0; i < NTEST; i++)
    {
        Number a = Number(+MIN / 2 - (random() & 0xFFFF));
        Number b = Number(-MIN / 2 + (random() & 0xFFFF));
        try
        {
            a - b;
            cout << RED << "Test failed!\n" << WHITE;
            a.dump();
            b.dump();
            printf("Should be overflowed");
        }
        catch (std::exception& e)
        {
            printTestExeptionInfo(e, a, b, "operator - lead to overflowing!");
        }
    }
    printf("\n");


    printf("Multiplication:\n");
    const i64 SQRT_MAX = sqrt(MAX);
    for (ui32 i = 0; i < NTEST; i++)
    {

        Number a = Number(SQRT_MAX + (random() & 0xFFFF) + 1);
        Number b = Number(SQRT_MAX + (random() & 0xFFFF) + 1);
        try
        {
            a * b;
            cout << RED << "Test failed!\n" << WHITE;
            a.dump();
            b.dump();
            printf("Should be overflowed");
        }
        catch (std::exception& e)
        {
            printTestExeptionInfo(e, a, b, "operator * lead to overflowing!");
        }
    }
    printf("\n");


}

/**
    \brief Релизация теста, эмулюрующий битую ссылку
*/
void t_nullptrInThis()
{
    try
    {
        Number* numPtr = nullptr;
        Number& brokenNumber = *numPtr;
        brokenNumber++;
    }
    catch (std::exception& e)
    {
        if (strcmp(e.what(), "this contain nullptr!"))
        {
            cout << RED << "Test failed!\n" << WHITE
                << "Cathed unexpected exception: "
                << e.what() << endl;
        }
        else
            cout << GREEN << "Ok " << WHITE;
    }
}

/**
    \brief Релизация теста, эмулюрующий деление на ноль
    \note  При выводе будут цвета двух типов:
           Малиновый отвечает тому, что деления на ноль не было и все ок
           Зеленый отвечает тому, что деление на ноль было перехвачено
*/
void t_db0()
{
    const i64 NTEST = 1024;
    for (ui64 i = 0; i < NTEST; i++)
    {
        i64 a = random();
        i64 b = (random() & 0xF) > 4 ? 0 : random() & 0xFFFF;

        try
        {
            Number(a) / Number(b);
            cout << LIGHTMAGENTA << "Ok " << WHITE;
        }
        catch (std::exception& e)
        {
            if (strcmp(e.what(), "division by zero!"))
            {
                cout << RED << "Test failed!\n" << WHITE
                    << "Cathed unexpected exception: "
                    << e.what() << endl;
                printf("a = %lld; b = %lld\n", a, b);
            }
            else
                cout << GREEN << "Ok " << WHITE;

        }
    }
}

void t_fib()
{
    const ui32 NTEST = 16;
    for (ui32 i = 0; i < NTEST; i++)
    {
        i64 index = (i >> 1) + (random() & 0xF);
        Number _fib = t_fib_Number(index);
        i64 fib = t_fib_i64(index);
        if (Number(fib) != _fib)
        {
            cout << RED << "Test failed!" << WHITE << endl;
            printf("Index: %lld\n _fib[%lld] = ", index);
            _fib.dump();
            printf("But it should be: fib[%lld] = %lld\n", index, fib);
        }
        else
            cout << GREEN << "Ok " << WHITE;

    }
}

/**
    \brief Тест эмулирующий проверку корректности выполнения
           арифметических операций
*/
void t_operators()
{
    srand(time(nullptr));
    const int NTEST = 64;
    enum Operations
    {
        OP_SUM,
        OP_SUB,
        OP_MUL,
        OP_DIV
    };
    const string op_names[]
    {
        "sum","sub","mul","div"
    };

    for (Operations op = OP_SUM; op != OP_DIV; op = (Operations)(op + 1))
        for (i32 i = 0; i < NTEST; i++)
        {
            i64 a = random();
            i64 b = random();
            i64 c = 0;
            Number _a = a;
            Number _b = b;
            Number _c = 0;
            if (op == OP_DIV && b == 0)
                _b = 1;
            switch (op)
            {
            case OP_SUM: _c = _a + _b; c = a + b; break;
            case OP_SUB: _c = _a - _b; c = a - b; break;
            case OP_MUL: _c = _a * _b; c = a * b; break;
            case OP_DIV: _c = _a / _b; c = a / b; break;
            default:break;
            }
            if (_c != Number(c))
            {
                cout << RED << "Test failed!" << WHITE << endl;
                cout << "a:"; _a.dump();
                cout << "b:"; _b.dump();
                cout << "c:"; _c.dump();
                printf("c should be: %lld\n", c);
                printf("operation: %s\n", op_names[op].c_str());
            }
            else
                cout << GREEN << "Ok " << WHITE;

        }
}


/**
    \brief Функция запускает тесты для структуры Number
*/
void testing_INT()
{
    try
    {
        printf("Test basic operators\n");
        t_operators();
        printf("\n");
    }
    catch (std::exception& e)
    {
        throw ChainException("Failed basic operators test", DEBUG_LOCATION_GEN, new exception(e));
    }
        

    try
    {
        printf("Test calculate fibbonacci numbers\n");
        t_fib();
        printf("\n");
    }
    catch (std::exception& e)
    {
        throw ChainException("Failed calculation fibbonacci test", DEBUG_LOCATION_GEN, new exception(e));
    }


    try
    {
        printf("Test division by zero\n");
        t_db0();
        printf("\n");
    }
    catch (std::exception& e)
    {
        throw ChainException("Failed division by zero test", DEBUG_LOCATION_GEN, new exception(e));
        //#define CHAINEXCEPTION(msg, ...) ChainException(msg, DEBUG_LOCATION_GEN, ##__VA_ARGS__ )
    }


    try
    {
        printf("Test store nullptr in \'this\'\n");
        t_nullptrInThis();
        printf("\n");
    }
    catch (std::exception& e)
    {
        throw ChainException("Failed store nullptr test", DEBUG_LOCATION_GEN, new exception(e));
    }


    try
    {
        printf("Test with overflowing\n");
        t_overflowing();
        printf("\n");
    }
    catch (std::exception& e)
    {
        throw ChainException("Failed overflowing test", DEBUG_LOCATION_GEN, new exception(e));
    }

    try
    {
        printf("Test with sorting array\n");
        t_sorting();
        printf("\n");
    }
    catch (std::exception& e)
    {
        throw ChainException("Failed sorting test", DEBUG_LOCATION_GEN, new exception(e));
    }

}
