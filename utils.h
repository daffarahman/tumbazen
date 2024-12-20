#ifndef _UTILS_H_
#define _UTILS_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

// String operations
char *util_trimStr(char *str);
char *util_lowerStr(char *str);
bool util_isSubstr(char *str, char *substr);
char *util_intToStr(int n);
double util_strToDouble(char *str);
int util_strLength(char *str);

#endif