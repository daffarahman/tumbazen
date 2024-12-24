#include "datetime.h"

dt_DateTime *dt_strToDate(char *str)
{
    dt_DateTime *date = (dt_DateTime *)malloc(sizeof(dt_DateTime));
    int buff_day, buff_month, buff_year;
    int buff_hour, buff_minute, buff_second;

    sscanf(strdup(str), "%d-%d-%d %d:%d:%d", &buff_day, &buff_month, &buff_year, &buff_hour, &buff_minute, &buff_second);

    date->day = buff_day;
    date->month = buff_month;
    date->year = buff_year;

    date->hour = buff_hour;
    date->minute = buff_minute;
    date->second = buff_second;

    return date;
}

dt_DateTime *dt_getTimeNow()
{
    time_t now;
    struct tm *timeNow;
    dt_DateTime *date = (dt_DateTime *)malloc(sizeof(dt_DateTime));

    time(&now);
    timeNow = localtime(&now);

    date->day = timeNow->tm_mday;
    date->month = timeNow->tm_mon + 1;
    date->year = timeNow->tm_year + 1900;
    date->hour = timeNow->tm_hour;
    date->minute = timeNow->tm_min;
    date->second = timeNow->tm_sec;

    return date;
}

char *dt_dateTimeToStr(dt_DateTime date)
{
    char *str = (char *)malloc(20 * sizeof(char));
    sprintf(str, "%02d-%02d-%04d %02d:%02d:%02d",
            date.day, date.month, date.year,
            date.hour, date.minute, date.second);
    return str;
}

int dt_getDaysBetween(dt_DateTime dt1, dt_DateTime dt2)
{
    int days1 = dt_dateTimeToDays(dt1);
    int days2 = dt_dateTimeToDays(dt2);

    return days2 - days1;
}

int dt_isLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int dt_daysInMonth(int month, int year)
{
    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && dt_isLeapYear(year))
        return 29;
    return days[month - 1];
}

int dt_dateTimeToDays(dt_DateTime dt)
{
    int days = 0;

    for (int year = 0; year < dt.year; year++)
        days += dt_isLeapYear(year) ? 366 : 365;

    for (int month = 1; month < dt.month; month++)
        days += dt_daysInMonth(month, dt.year);

    days += dt.day;

    return days;
}

dt_DateTime *dt_daysToDateTime(int days)
{
    dt_DateTime *dt = (dt_DateTime *)malloc(sizeof(dt_DateTime));
    int year = 0;

    while (days >= (dt_isLeapYear(year) ? 366 : 365))
    {
        days -= dt_isLeapYear(year) ? 366 : 365;
        year++;
    }
    dt->year = year;

    // Calculate the month
    int month = 1;
    while (days >= dt_daysInMonth(month, year))
    {
        days -= dt_daysInMonth(month, year);
        month++;
    }
    dt->month = month;

    dt->day = days + 1;

    dt->hour = 0;
    dt->minute = 0;
    dt->second = 0;

    return dt;
}