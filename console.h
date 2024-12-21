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
#include <conio.h>
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

// get the size of your console window, x = width, y = height
con_Coord *con_getSize();
// clears the screen
void con_clearScr();

// never ever use scanf, this is sooo much better, just like in python
int con_inputInt();
// again, use this instead scanf
char *con_inputStr();
// love this one, when you type your password, it only shows *****, amazing
char *con_inputPwd();
// press any key to continue
void con_anyKey();

// set the echo to on
void con_echoOn();
// set the echo to off
void con_echoOff();

// output string a color, no it doesn't support formatting.. yet
void con_printColor(char *str, char *color);

#endif