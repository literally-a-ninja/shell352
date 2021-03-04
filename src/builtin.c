#include <stdio.h>
#include <string.h>

#include "buongiorno/builtin.h"

#include "builtin_cd.c"
#include "builtin_exit.c"

// clang-format off
const struct builtin_t builtin_cmds[BUILTIN_CMDS_LENGTH] = {
    {"quit",    &builtin_exit},
    {"exit",    &builtin_exit},
    {"cd",      &builtin_cd}
};
// clang-format on

builtin_callback_t *findBuiltinCmd (char *name)
{
    unsigned index;
    for (index = 0; index < BUILTIN_CMDS_LENGTH; index++)
    {
        if (!strcmp (builtin_cmds [index].name, name))
            return builtin_cmds [index].cb;
    }

    return 0;
}
