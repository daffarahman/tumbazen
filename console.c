#include "console.h"

con_Coord *con_getSize()
{
    con_Coord *new_coor = (con_Coord *)malloc(sizeof(con_Coord));

#ifndef _WIN32
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    new_coor->x = w.ws_col;
    new_coor->y = w.ws_row;
#else
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    new_coor->x = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    new_coor->y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#endif

    return new_coor;
}

void con_clearScr()
{
#ifndef _WIN32
    system("clear");
#else
    system("cls");
#endif
}

int con_inputInt()
{
    char *buff = (char *)malloc(sizeof(char) * 255);
    if (fgets(buff, 255, stdin) == NULL)
        return 0;

    buff[strcspn(buff, "\r\n")] = 0;

    return atoi(buff);
}

char *con_inputStr()
{
    char *buff = (char *)malloc(sizeof(char) * 255);
    if (fgets(buff, 255, stdin) == NULL)
        return strdup("");

    buff[strcspn(buff, "\r\n")] = 0;

    return strdup(util_trimStr(buff));
}

char *con_inputPwd()
{
    char *buff = (char *)malloc(sizeof(char) * 255);

    int i = 0;
    char ch;

    fflush(stdout);

    con_echoOff();

    while (1)
    {
#ifdef _WIN32
        ch = getch();
#else
        ch = getchar();
#endif
        if (ch == '\n' || ch == '\r')
            break;
        else if (ch == '\b' || ch == 127)
        {
            if (i > 0)
            {
                printf("\b \b");
                fflush(stdout);
                i--;
            }
        }
        else if (i < 255 - 1)
        {
            buff[i++] = ch;
            printf("*");
            fflush(stdout);
        }
    }

    buff[i] = '\0';
    con_echoOn();
    printf("\n");

    return buff;
}

void con_anyKey()
{
    char e = 0;
    while (e != '\r' && e != '\n')
        e = getchar();
}

// echos
void con_echoOn()
{
#ifdef _WIN32
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode | ENABLE_ECHO_INPUT);
#else
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    tty.c_lflag |= ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
#endif
}

void con_echoOff()
{
#ifdef _WIN32
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode & ~ENABLE_ECHO_INPUT);
#else
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    tty.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
#endif
}

void con_printColor(char *str, char *color)
{
    printf("%s%s%s", color, str, FG_DEFAULT);
}