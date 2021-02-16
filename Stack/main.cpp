#include <cstdlib>
#include "Tools/Logger.h"

void start_test();

int main()
{
    logger.init("log.log");
    start_test();
    system("pause");
    return 0;
}