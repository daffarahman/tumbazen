#include "utils.h"

char *util_trimStr(char *str)
{
    char *res = strdup(str);
    char *start = res;
    char *end;

    while (isspace((unsigned char)*start))
        start++;

    if (*start == '\0')
    {
        res[0] = '\0';
        return res;
    }

    end = start + strlen(start) - 1;

    while (end > start && isspace((unsigned char)*end))
        end--;

    *(end + 1) = '\0';

    memmove(res, start, end - start + 2);

    return res;
}

char *util_lowerStr(char *str)
{
    char *lowered = strdup(str);
    int len = strlen(lowered);
    for (int i = 0; i < len; i++)
        if (isupper(lowered[i]))
            lowered[i] = tolower(lowered[i]);

    return lowered;
}

bool util_isSubstr(char *str, char *substr)
{
    char *res = strstr(strdup(str), strdup(substr));
    if (res)
        return true;
    return false;
}

char *util_intToStr(int n)
{
    char *res = (char *)malloc(sizeof(char) * 128);
    snprintf(res, 128, "%d", n);
    return res;
}

double util_strToDouble(char *str)
{
    double d = 0.0;
    sscanf(str, "%lf", &d);
    return d;
}

int util_strLength(char *str)
{
    int res = 0;
    while (str[res] != '\0')
    {
        res++;
    }
    res++;
    return res;
}