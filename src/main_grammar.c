#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "buongiorno/common.h"
#include "buongiorno/grammar.h"

array_t *parse_input_runs (char *input)
{
    run_t **array = malloc (0);
    size_t length = 0;
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

            run_t *run = ctor_run_t ();
            parse_input_cmds (ptrContext, run);

            if (!bNextToLast)
            {
                *ptr = c;
            }

            // Attach run to array
            array            = realloc (array, sizeof (run_t *) * (length + 1));
            array [length++] = run;

            // Set to null, next char will reset this properly.
            ptrContext = 0;

            if (*ptr == '&')
                ptr++;
        }
    }

    return ctor_arr_wrapper ((void **)array, length);
}

void parse_input_cmds (char *input, run_t *run)
{
    char *ptr;
    char *ptrContext;

    cmd_t *cmd = ctor_cmd_t ();
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
            unsigned length = ptr - ptrContext + (bNextToLast ? 1 : 0);
            cmd->executable = malloc (sizeof (char) * length);
            strncpy (cmd->executable, ptrContext, length);

            run->commands [run->commands_size++] = cmd;
            ptrContext                           = 0;
            cmd                                  = ctor_cmd_t ();
        }
    }

    free (cmd);
}

void printRuns (run_t **runs, size_t runs_size)
{
    unsigned i;
    for (i = 0; i < runs_size; i++)
    {
        run_t *run = runs [i];
        printf ("Run %d:\n", i + 1);

        unsigned c;
        for (c = 0; c < run->commands_size; c++)
        {
            cmd_t *cmd = run->commands [c];
            printf ("%s%s\n", c ? " > " : "    ", cmd->executable);
        }

        printf ("\n");
    }
}

int main (int argc, char *argv [], char *envp [])
{
    if (argc != 2)
    {
        printf ("Usage: %s PHRASE\n", argv [0]);
        return 0;
    }

    char *input = argv [1];

    // First step, find the runs of the input.
    array_t *runs = parse_input_runs (input);
    printRuns ((run_t **)runs->array, runs->length);

    return 0;
}

