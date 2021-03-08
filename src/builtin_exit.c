#include <stdlib.h>

#include "buongiorno/common.h"
#include "shell352.h"

int builtin_exit (cmd_t *cmd, struct environment *e)
{
    g_shellStatus |= SHELL_DIE;
    return 0;
}
