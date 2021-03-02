#include "Tools/Types.h"
#include "Tools/ChainException.h"
#include "Tools/Console.h"
#include "Number.h"

using namespace std;

#include "Tools/StackTracerMin.h"
int __stack_level = 0;



void testing_INT();


int main()
{
    try
    {
        testing_INT();
    }
    catch (ChainException& e)
    {
        printf("Unexpected excpetion has cathed: %s", e.what());
    }
    system("pause");
    return 0;
}
