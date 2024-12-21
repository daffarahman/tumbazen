#ifndef _UTILS_H_
#define _UTILS_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

// removes whitespaces at the beginning and the end of string
char *util_trimStr(char *str);
// lower the string
char *util_lowerStr(char *str);
// check if substr is inside the str
bool util_isSubstr(char *str, char *substr);
// converts int to char* because itoa doesn't work for some reason
char *util_intToStr(int n);
// converts char* to double
double util_strToDouble(char *str);
// just use strlen().. please
int util_strLength(char *str);

#endif