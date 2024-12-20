#include "visual.h"

void vis_printListMenu(int count, ...)
{
    va_list vlptr;

    va_start(vlptr, count);

    for (int i = 0; i < count; i++)
        vis_printListOption(util_intToStr(i + 1), va_arg(vlptr, char *));

    va_end(vlptr);
}

void vis_printListOption(char *idc, char *str)
{
    printf("%s[%s%s%s] %s%s\n", FG_YELLOW, FG_CYAN, idc, FG_YELLOW, FG_DEFAULT, str);
}

void vis_printBars(char bar, int count)
{
    for (int i = 0; i < count; i++)
        printf("%c", bar);
}