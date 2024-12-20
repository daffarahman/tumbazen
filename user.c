#include "user.h"

User *usr_newUser()
{
    User *new_user = (User *)malloc(sizeof(User));
    new_user->username = (char *)malloc(sizeof(char) * 255);
    new_user->is_login = false;
    new_user->is_seller = false;
    return new_user;
}