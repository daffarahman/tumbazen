#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

typedef struct
{
    char *var;
    char *val;
} Var;

typedef struct
{
    int var_count;
    Var *variables;
} Config;

Config *conf_newConfig();
Var *conf_newVar();
void conf_addVar(Config *conf, Var var);
void conf_addFromFile(Config *conf, char *path);
Var *conf_strToVar(char *str);
void conf_printConfig(Config conf);
Var *conf_getVar(Config conf, char *var);

#endif