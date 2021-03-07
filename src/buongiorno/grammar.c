#include <stdlib.h>

#include "grammar.h"

#define CMD_MAX_PIPED_CMDS 15

run_t *ctor_run_t ()
{
    run_t *run         = malloc (sizeof (run_t));
    run->commands      = malloc (sizeof (cmd_t *) * CMD_MAX_PIPED_CMDS);
    run->commands_size = 0;

    return run;
}

cmd_t *ctor_cmd_t ()
{
    cmd_t *cmd      = malloc (sizeof (cmd_t));
    cmd->executable = 0;
    cmd->props      = ctor_cmd_props_t ();
    cmd->args       = malloc (sizeof (char *));
    cmd->args_size  = 0;

    return cmd;
}

cmd_props_t *ctor_cmd_props_t ()
{
    return malloc (sizeof (cmd_props_t));
}

array_t *ctor_arr_wrapper (void **contents, size_t length)
{
    array_t *wrapper = malloc (sizeof (array_t));
    wrapper->array   = contents;
    wrapper->length  = length;

    return wrapper;
}
