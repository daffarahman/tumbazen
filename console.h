#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include <stdio.h>
#include <stdlib.h>

#ifndef _WIN32
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#else
#include <windows.h>
#endif

#include "utils.h"

// background colors
#define BG_BLACK "\x1b[40m"
#define BG_RED "\x1b[41m"
#define BG_GREEN "\x1b[42m"
#define BG_YELLOW "\x1b[43m"
#define BG_BLUE "\x1b[44m"
#define BG_MAGENTA "\x1b[45m"
#define BG_CYAN "\x1b[46m"
#define BG_WHITE "\x1b[47m"

// foreground colors
#define FG_BLACK "\x1b[30m"
#define FG_RED "\x1b[31m"
#define FG_GREEN "\x1b[32m"
#define FG_YELLOW "\x1b[33m"
#define FG_BLUE "\x1b[34m"
#define FG_MAGENTA "\x1b[35m"
#define FG_CYAN "\x1b[36m"
#define FG_WHITE "\x1b[37m"

// defaults
#define FG_DEFAULT FG_WHITE
#define FG_PROMPT FG_GREEN
#define FG_ERROR FG_RED

typedef struct
{
    int x;
    int y;
} con_Coord;

con_Coord *con_getSize();
void con_clearScr();

// input
int con_inputInt();
char *con_inputStr();
char *con_inputPwd();
void con_anyKey();

// echo
void con_echoOn();
void con_echoOff();

// output
void con_printColor(char *str, char *color);

#endif