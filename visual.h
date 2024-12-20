#ifndef _VISUAL_H_
#define _VISUAL_H_

#include "foundation.h"
#include "console.h"

#define V_BAR '-'

void vis_printListMenu(int count, ...);
void vis_printListOption(char *idc, char *str);
void vis_printBars(char bar, int count);

#endif