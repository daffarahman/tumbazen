#include "random.h"

int rd_getRangeInt(int start, int end)
{
    srand(time(0));
    return start + rand() % (end - start + 1);
}