#include <string.h>
#include <stdio.h>

#ifndef H_BUILTIN
#include "builtin.h"
#endif

#include "builtin/builtin_exit.c"

#define BUILTIN_CMDS_LENGTH 2

/**
 * Array holds all builtin command fn pointers
 **/
const struct builtin_t builtin_cmds[BUILTIN_CMDS_LENGTH] = {
    {"quit", &builtin_exit},
    {"exit", &builtin_exit}
};

builtin_callback_t *findBuiltinCmd(char *name)
{
    unsigned index;
    for (; index < BUILTIN_CMDS_LENGTH; index++)
    {
        if (! strcmp(builtin_cmds[index].name, name))
            return builtin_cmds[index].cb;
    }

    return 0;
}
