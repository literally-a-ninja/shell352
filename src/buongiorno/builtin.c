#include <string.h>
#include <stdio.h>

#include "builtin.h"
#include "builtin/builtin_exit.h"

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
