#ifndef _DATETIME_H_
#define _DATETIME_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct
{
    int day;
    int month;
    int year;
    int hour;
    int minute;
    int second;
} dt_DateTime;

dt_DateTime *dt_strToDate(char *str);
dt_DateTime *dt_getTimeNow();

char *dt_dateTimeToStr(dt_DateTime date);

int dt_getDaysBetween(dt_DateTime dt1, dt_DateTime dt2);
int dt_isLeapYear(int year);
int dt_daysInMonth(int month, int year);
int dt_dateTimeToDays(dt_DateTime dt);
dt_DateTime *dt_daysToDateTime(int days);

#endif