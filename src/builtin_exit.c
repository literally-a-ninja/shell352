#include <stdlib.h>
#include <unistd.h>

#include "buongiorno/common.h"
#include "shell352.h"

int builtin_exit (cmd_t *cmd, struct environment *e)
{
    close (STDIN_FILENO);
    return 0;
}
