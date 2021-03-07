#include <fcntl.h>
#include <signal.h>

typedef struct cmd_props_t
{
    char *redirect_to;
    unsigned redirect_to_length;
    char *redirect_from;
    unsigned redirect_from_length;
    int bg : 1;
} cmd_props_t;

typedef struct cmd_t
{
    cmd_props_t *props;
    char *executable;
    unsigned executable_length;
    char **args;
    unsigned args_size;
} cmd_t;

typedef struct run_t
{
    cmd_t **commands;
    unsigned commands_size;
} run_t;

typedef struct array_t
{
    void **array;
    unsigned length;
} array_t;

/**
 * Constructs and allocates a run of commands.
 **/
run_t *ctor_run_t ();

/**
 * Constructs and allocates a command.
 **/
cmd_t *ctor_cmd_t ();

/**
 * Constructs and allocates cmd props.
 **/
cmd_props_t *ctor_cmd_props_t ();

/**
 * Constructs and allocates an array wrapper struct.
 **/
array_t *ctor_arr_wrapper (void **contents, size_t length);

array_t *parse_input_runs (char *input);
void parse_input_cmds (char *input, run_t *runs);
