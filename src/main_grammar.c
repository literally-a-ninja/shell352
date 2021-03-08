#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "buongiorno/common.h"
#include "buongiorno/grammar.h"
#include "buongiorno/parse.h"
#include "buongiorno/string.h"

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

            unsigned a;
            for (a = 0; a < cmd->args_size; a++)
            {
                printf ("%s%s", a ? " " : "", cmd->args [a]);
            }
            printf ("\n");
        }
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
    array_t *runs = B_parse_input_runs (input, strlen (input));
    printRuns ((run_t **)runs->array, runs->length);

    return 0;
}

