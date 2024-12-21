#ifndef _VISUAL_H_
#define _VISUAL_H_

#include "foundation.h"
#include "console.h"

#define V_BAR '-'

// print list menu, because we have a lot of options for user, so this is just what we need
// 1.A
// 2.B
// 3.C
void vis_printListMenu(int count, ...);
// this is for custom option
void vis_printListOption(char *idc, char *str);
// print bars with the lenght of count
void vis_printBars(char bar, int count);

#endif