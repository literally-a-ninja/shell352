#include <stdio.h>
#include <stdlib.h>

#include "buongiorno/common.h"
#include "shell352.h"

int builtin_fg (cmd_t *cmd, struct environment *e)
{
    fprintf (stdout, "fg: There are no suitable jobs.\n");
    return 0;
}

int builtin_bg (cmd_t *cmd, struct environment *e)
{
    fprintf (stdout, "bg: There are no suitable jobs.\n");
    return 0;
}

int builtin_jobs (cmd_t *cmd, struct environment *e)
{
    fprintf (stdout, "jobs: There are no jobs.\n");
    return 0;
}
