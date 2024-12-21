#ifndef _USER_H
#define _USER_H

#include <stdbool.h>
#include <stdlib.h>

// stores current user data, used for authetication and validating user data
typedef struct
{
    char *username;
    bool is_login;
    bool is_seller;
} User;

// perfectly said
User *usr_newUser();

#endif