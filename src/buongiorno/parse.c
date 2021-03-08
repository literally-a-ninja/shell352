#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"
#include "string.h"

array_t *B_parse_input_runs (char *input, unsigned length)
{
    run_t **array    = malloc (0);
    size_t arrLength = 0;
    char *ptr;
    char *ptrContext;

    for (ptr = input, ptrContext = 0; *ptr; ptr++)
    {
        unsigned bNextToLast = !*(ptr + 1);

        // ptr set to null on new context
        if (!ptrContext)
            ptrContext = ptr;

        if (
            /* Matches ; */
            *ptr == ';'

            /* Matches && */
            || (!bNextToLast && *ptr == *(ptr + 1) && *ptr == '&')

            /* Matches end of input */
            || bNextToLast)
        {
            char c = 0;
            if (!bNextToLast)
            {
                c    = *ptr;
                *ptr = 0;
            }

            unsigned subLength = ptr - ptrContext + (bNextToLast ? 1 : 0);


            // Remove CR
            ptrContext[subLength - 1] = 0;

            char *line = malloc (sizeof (char) * subLength);
            strncpy (line, ptrContext, subLength);
            run_t *run = B_ctor_run_t (line);

            B_parse_input_cmds (ptrContext, subLength, run);

            if (!bNextToLast)
            {
                *ptr = c;
            }

            // Attach run to array
            array = realloc (array, sizeof (run_t *) * (arrLength + 1));
            array [arrLength++] = run;

            // Set to null, next char will reset this properly.
            ptrContext = 0;

            if (*ptr == '&')
                ptr++;
        }
    }

    return B_ctor_arr_wrapper ((void **)array, arrLength);
}

void B_parse_input_cmds (char *input, unsigned length, run_t *run)
{
    char *ptr;
    char *ptrContext;

    cmd_t *cmd = B_ctor_cmd_t ();
    for (ptr = input, ptrContext = 0; *ptr; ptr++)
    {
        unsigned bNextToLast = !*(ptr + 1);

        // ptr set to null on new context
        if (!ptrContext)
            ptrContext = ptr;

        if (
            /* matches | */
            *ptr == '|'

            /* matches end of input */
            || bNextToLast)
        {

            unsigned subLength = ptr - ptrContext + (bNextToLast ? 1 : 0);
            array_t *a         = B_parse_input_args (ptrContext, subLength);

            if (!a->length)
            {
                free (a);
                continue;
            }

            array_t *exec   = (array_t *)a->array [0];
            cmd->executable = malloc (sizeof (char) * exec->length);
            cmd->args       = malloc (sizeof (char *) * a->length);
            cmd->args_size  = 0;
            strncpy (cmd->executable, (char *)exec->array, exec->length);

            unsigned i;
            for (i = 0; i < a->length; i++)
            {
                array_t *arg = (array_t *)a->array [i];
                char *value  = (char *)arg->array;

                if (B_str_isredirect (value))
                {
                    cmd_redirect_t *redir;

                    if (arg->length <= 2)
                    {
                        array_t *nextArg = (array_t *)a->array [++i];
                        redir            = B_ctor_cmd_redirect_t_tokenized (
                            value, (char *)nextArg->array, nextArg->length);
                    }
                    else
                    {
                        redir = B_ctor_cmd_redirect_t (value, arg->length);
                    }

                    cmd_redirect_t **ptr =
                        (redir->bOutput ? &cmd->props->redirect_to
                                        : &cmd->props->redirect_from);

                    *ptr = redir;
                    continue;
                }

                if (!strcmp (value, "&"))
                {
                    cmd->props->bg = 1;
                    break;
                }

                cmd->args [cmd->args_size] =
                    malloc (sizeof (char) * arg->length);
                strcpy (cmd->args [cmd->args_size++], value);
            }

            // Resize to remove redirects and special operators
            cmd->args = realloc (cmd->args, sizeof (char *) * cmd->args_size);

            run->commands [run->commands_size++] = cmd;

            free (a);
            ptrContext = 0;
            cmd        = B_ctor_cmd_t ();
        }
    }

    free (cmd);
}
