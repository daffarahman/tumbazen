#include "config.h"

Config *conf_newConfig()
{
    Config *new_config = (Config *)malloc(sizeof(Config));
    new_config->var_count = 0;
    new_config->variables = NULL;
    return new_config;
}

Var *conf_newVar()
{
    Var *new_var = (Var *)malloc(sizeof(Var));
    new_var->val = (char *)malloc(sizeof(char) * 128);
    new_var->var = (char *)malloc(sizeof(char) * 128);
    return new_var;
}

void conf_addVar(Config *conf, Var var)
{
    conf->var_count++;
    conf->variables = (Var *)realloc(conf->variables, sizeof(Var) * conf->var_count);
    conf->variables[conf->var_count - 1] = var;
}

void conf_addFromFile(Config *conf, char *path)
{
    FILE *fptr = fopen(path, "r");
    if (!fptr)
        return;

    char *buff = (char *)malloc(sizeof(char) * 255);
    while (fgets(buff, 255, fptr))
    {
        conf_addVar(conf, *conf_strToVar(strdup(util_trimStr(buff))));
    }

    fclose(fptr);
}

Var *conf_strToVar(char *str)
{
    Var *new_var = conf_newVar();
    new_var->var = strtok(strdup(str), "=");
    new_var->val = strtok(NULL, "=");
    return new_var;
}

void conf_printConfig(Config conf)
{
    for (int i = 0; i < conf.var_count; i++)
        printf("%s = %s\n", conf.variables[i].var, conf.variables[i].val);
}

Var *conf_getVar(Config conf, char *var)
{
    for (int i = 0; i < conf.var_count; i++)
        if (strcmp(conf.variables[i].var, var) == 0)
            return &conf.variables[i];

    return NULL;
}