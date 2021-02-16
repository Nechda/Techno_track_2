#include "Stack.hpp"
#include <stack>
#include <ctime>
#include <iostream>
#include <string>

using namespace std;

const ui32 N_TESTS = 64;


static inline ui32 random(ui32 a, ui32 b)
{
    if (a > b) a ^= b ^= a;
    return rand() % (b - a + 1) + a;
}

static inline ui32 random_int()
{
    return rand() % 10000;
}


/*
\brief Реализация простой структуры Student, для тестирования стека
*/
struct Student
{
    string name;
    ui32 height;
    Student() : name(""), height(0) {}
    Student(string _name, ui32 _height) : name(_name), height(_height) {}
    friend std::ostream& operator<< (std::ostream& out, const Student& foo);
};

std::ostream& operator<< (std::ostream& out, const Student& foo)
{
    out << foo.name << " Height:" << foo.height << "cm" << endl;
    return out;
}

void TypeTrace<Student>::print(const Student& student)
{
    printf("Name: %s, Height: %u cm", student.name.c_str(), student.height);
    fflush(stdout);
}


bool operator != (const Student& a, const Student& b)
{
    return a.height != b.height || a.name != b.name;
}

/*
\brief Функция генерации студентов :)
*/
static inline Student genStudent()
{
    static string names[] =
    {
        "Ivan", "Alexander", "Maxim", "Dmitry",
        "Anna", "Sofia", "Maria", "Anastasia"
    };

    return Student(names[rand() % 8], rand() % 20 + 155);
}


/*
\brief Функция производит дамп двух стеков
*/
template<typename T>
static void printInfo(stack<T>& std_stk, Stack<T>& my_stk)
{
    cout << "std stack contains:" << endl;
    while (!std_stk.empty())
    {
        cout << std_stk.top() << " ";
        std_stk.pop();
    }

    cout << "dumping my stack:" << endl;
    my_stk.dump(); 
}

template<typename T>
using pFunc = T(*)();

/*
\brief   Функция тестирования кастомного стека
\detail  Функция создает набор случайных данных, исходя из функции generate,
         которая передается в качестве параметра. После чего в два стека stl & мой стек
         записываются одинаковые данные. Затем данные поочередно вытаскиваются из двух стеков
         при этом происходит сравнение данных на вершинах.
\params  [in]  generate   Указатель на функцию, которая генерирует новый элемент стека
*/
template<typename T>
void stack_test(pFunc<T> generate)
{
    srand(time(0));
    stack<T> std_stack;
    Stack<T> my_stack;
    printf("Start testing for stack<%s>\n", typeid(T).name());

    my_stack.dump();

    for (ui32 i = 0; i < N_TESTS; i++)
    {
        i8 size = random(0, 15);
        while (size--)
        {
            T elem = generate();
            std_stack.push(elem);
            my_stack.push(elem);
        }

        while (!std_stack.empty())
        {
            if (std_stack.top() != my_stack.top())
            {
                printf("Test[%d] failed!\n", i + 1);
                cout << std_stack.top() << " & " << my_stack.top() << endl;
                printInfo(std_stack, my_stack);
                return;
            }
            std_stack.pop();
            my_stack.pop();
        }
        printf("Ok ", i+1);
    }
    printf("\n");
}


/*
\brief  Функция эмулирует атаки на канареечные переменные
*/
template<typename T>
void stack_test_attack()
{
    Stack<T> my_stack;

    my_stack.CANARY_LEFT = 0;
    if (my_stack.status() == STACK_ATTACK_ON_LEFT_CANARY)
        printf("Ok ");
    else
        printf("Stack was destroyed from the left size...\n");
    my_stack.CANARY_LEFT = MAGIC_NUMBER;

    my_stack.CANARY_RIGHT = 0;
    if (my_stack.status() == STACK_ATTACK_ON_RIGHT_CANARY)
        printf("Ok ");
    else
        printf("Stack was destroyed from the rihgt size...\n");
    my_stack.CANARY_RIGHT = MAGIC_NUMBER;
}

/*
\brief Функция, тестирующий стек на разных типах данных
*/
void start_test()
{
    stack_test<ui32>(random_int);
    stack_test<Student>(genStudent);

    stack_test_attack<ui64>();
}