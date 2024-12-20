#ifndef _USER_H
#define _USER_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct
{
    char *username;
    bool is_login;
    bool is_seller;
} User;

User *usr_newUser();

#endif