#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grammar.h"
#include "parse.h"
#include "string.h"

#define CMD_MAX_PIPED_CMDS 15

run_t *B_ctor_run_t (char *line)
{
    run_t *run         = malloc (sizeof (run_t));
    run->commands      = malloc (sizeof (cmd_t *) * CMD_MAX_PIPED_CMDS);
    run->line          = line;
    run->commands_size = 0;

    return run;
}

cmd_t *B_ctor_cmd_t ()
{
    cmd_t *cmd      = malloc (sizeof (cmd_t));
    cmd->executable = 0;
    cmd->props      = B_ctor_cmd_props_t ();
    cmd->args       = malloc (sizeof (char *));
    cmd->args_size  = 0;

    return cmd;
}

cmd_props_t *B_ctor_cmd_props_t ()
{
    return malloc (sizeof (cmd_props_t));
}

cmd_redirect_t *B_ctor_cmd_redirect_t_tokenized (char *firstToken,
                                                 char *secondToken,
                                                 unsigned secondLength)
{
    cmd_redirect_t *redirect = malloc (sizeof (cmd_redirect_t));
    char bFirstArgument      = '0' <= *firstToken && '2' >= *secondToken;
    redirect->bOutput        = *(firstToken + bFirstArgument) == '>';
    FILE *defaultPipe        = redirect->bOutput ? stdout : stdin;

    redirect->pipe        = defaultPipe;
    redirect->dest        = secondToken;
    redirect->dest_length = secondLength;

    if (bFirstArgument)
    {
        switch (*firstToken)
        {
        case '0':
            redirect->pipe = stdin;
            break;
        case '1':
            redirect->pipe = stdout;
            break;
        case '2':
            redirect->pipe = stderr;
            break;
        }
    }

    return redirect;
}

cmd_redirect_t *B_ctor_cmd_redirect_t (char *input, unsigned length)
{
    char bFirstArgument = '0' <= *input && '2' >= *input;

    array_t *dest = B_parse_input_string (input + bFirstArgument + 1,
                                          length - bFirstArgument - 1);

    return B_ctor_cmd_redirect_t_tokenized (input, (char *)dest->array,
                                            dest->length);
}

array_t *B_parse_input_string (char *input, unsigned length)
{
    char c;
    unsigned i            = 0;
    char cStringDelimiter = 0;
    char bEscaped         = 0;

    for (c = *input; i < length; c = input [++i])
    {
        if (bEscaped)
        {
            bEscaped = 0;
            continue;
        }

        switch (c)
        {
        case '\\':
            bEscaped = 1;
            continue;

        case '\'':
        case '\"':
            // No current delimiter, set and continue.
            if (!cStringDelimiter)
            {
                cStringDelimiter = c;
            }

            // We matched beginning delimiter.
            else if (cStringDelimiter == c)
            {
                cStringDelimiter = 0;
            }

            continue;

        case ' ':
            if (!cStringDelimiter)
                break;

        default:
            continue;
        }

        break;
    }

    char *str = malloc (sizeof (char) * (i + 1));
    strncpy (str, input, i);
    *(str + i) = 0;

    return B_ctor_arr_wrapper (str, i);
}

array_t *B_parse_input_args (char *input, unsigned length)
{
    unsigned argsLength = 0;
    array_t **args      = malloc (sizeof (array_t *));

    array_t *arg;
    char *strPtr;

    for (

        /* Setup initial pointer */
        strPtr = input, arg = B_parse_input_string (strPtr, length);

        /* Stop if we're out of room */
        length;

        /* Reset arg to next parsed token */
        strPtr = (strPtr + (arg ? arg->length : 1)),
        arg    = B_parse_input_string (strPtr, length)

    )
    {

        // Whitespace
        if (!arg->length)
        {
            free (arg);
            arg = 0;

            length--;
            continue;
        }

        args = realloc (args, sizeof (array_t *) * (++argsLength));
        args [argsLength - 1] = arg;
        length -= arg->length;
    }

    return B_ctor_arr_wrapper (args, argsLength);
}

array_t *B_ctor_arr_wrapper (void *contents, size_t length)
{
    array_t *wrapper = malloc (sizeof (array_t));
    wrapper->array   = contents;
    wrapper->length  = length;

    return wrapper;
}

