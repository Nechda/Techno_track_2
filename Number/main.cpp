п»ї#include "Tools/Types.h"
#include "Tools/ChainException.h"
#include "Tools/Console.h"
#include "Number.h"
#include "Dot.h"

using namespace std;

#include "Tools/StackTracerMin.h"
int StackTracer::stack_level = 0;


void testing_INT();

void start_testing()
{
    try
    {
        testing_INT();
    }
    catch (ChainException& e)
    {
        printf("Unexpected excpetion has cathed: %s", e.what());
    }
}



Number sqr(Number a)
{$1 $PRNT
    return a*a;
}

Number squaredLenOfVector(const Number& a, const Number& b)
{$1 $PRNT
    return a*a + sqr(b);
}

int main()
{
    $1 $PRNT
    #define VAR(type, name, value) type name(value, #name)
    {
        VAR(Number, a,  10);
        VAR(Number, b, 100);
        VAR(Number, c,  0);
        c = squaredLenOfVector(a, b);
    }

    dot::generateGraph("example.dot");
    
    system("pause");
    return 0;
}
