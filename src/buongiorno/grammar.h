#ifndef HB_GRAMMAR
#define HB_GRAMMAR

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>

#define CARRAIGE_RETURN '\n'
#define REDIRECT_OUT_OP '>'
#define REDIRECT_IN_OP '<'
#define PIPE_OP '|'
#define BG_OP '&'

typedef struct cmd_redirect_t
{
    FILE *pipe;
    char *dest;
    unsigned dest_length;
    char bOutput;
} cmd_redirect_t;

typedef struct cmd_props_t
{
    cmd_redirect_t *redirect_to;
    cmd_redirect_t *redirect_from;
    int bg : 1;
} cmd_props_t;

/* Holds a single command. */
typedef struct cmd_t
{
    /* Properties set by command parser */
    cmd_props_t *props;
    /* Executable path */
    char *executable;
    /* Executable path length */
    unsigned executable_length;
    /* Arguments array */
    char **args;
    /* Arguments array length */
    unsigned args_size;
} cmd_t;

/* Container for a run of commands seperated by a pipe */
typedef struct run_t
{
    /* Original line provided by stdin */
    char *line;
    /* Parsed sequence of commands */
    cmd_t **commands;
    /* Amount of commands reported */
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
run_t *B_ctor_run_t ();

/**
 * Constructs and allocates a command.
 **/
cmd_t *B_ctor_cmd_t ();

/**
 * Constructs and allocates cmd props.
 **/
cmd_props_t *B_ctor_cmd_props_t ();

/**
 * Constructs and allocates cmd redirect.
 **/
cmd_redirect_t *B_ctor_cmd_redirect_t (char *input, unsigned length);

/**
 * Constructs and allocates cmd redirect using two tokens.
 **/
cmd_redirect_t *B_ctor_cmd_redirect_t_tokenized (char *firstToken,
                                                 char *secondToken,
                                                 unsigned secondLength);

/**
 * Constructs and allocates cmd props.
 **/
cmd_props_t *B_ctor_cmd_props_t ();

/**
 * Constructs and allocates an array wrapper struct.
 **/
array_t *B_ctor_arr_wrapper (void *contents, size_t length);
#endif
