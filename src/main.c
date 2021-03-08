#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

// buongiornolib gets loaded first!
#include "buongiorno/common.h"
#include "buongiorno/grammar.h"
#include "buongiorno/parse.h"
#include "buongiorno/path.h"
#include "buongiorno/string.h"

#include "builtin.c"
#include "exec.c"
#include "shell352.h"

void recieve (int iSignal)
{
    switch (iSignal)
    {
    /* Foward SIGTSTP to the currently running forground process. */
    case SIGTSTP:
    case SIGTERM:
    case SIGQUIT:
        if (!g_pidFg)
            break;
        kill (g_pidFg, iSignal);
        break;

    /* Double print new line to let the user know we're done. */
    case SIGINT:
        if (g_pidFg)
            kill (g_pidFg, iSignal);
        else
        {
            printf ("\n");
            g_shellStatus |= SHELL_DIE;
        }
        break;

    default:
        return;
        break;
    }

    printf ("\n");
    signal (iSignal, &recieve);
}

void init ()
{
    signal (SIGINT, &recieve);
    signal (SIGTSTP, &recieve);
    signal (SIGQUIT, &recieve);
    signal (SIGTERM, &recieve);

    g_env          = malloc (sizeof (struct environment));
    g_env->m_ptrWd = malloc (sizeof (char) * PATH_MAX_DIR_LENGTH);
    getcwd (g_env->m_ptrWd, PATH_MAX_DIR_LENGTH);
    // g_env->m_ptrWd = realloc(&g_env->m_ptrWd, sizeof(char) *
    // B_strlen(g_env->m_ptrWd));

    g_pidFg = 0;
    g_run   = malloc (sizeof (run_t));
}

void shutdown ()
{
    free (g_run);
    free (g_env->m_ptrWd);
    free (g_env);

    pthread_join (g_inputThread, NULL);

#ifdef DEBUG
    printf (
        "D: shutdown() : Successfully freed all resources, returning to main() "
        "for termination.\n");
#endif
}

void command (cmd_t *cmd, char **envp)
{
    builtin_callback_t *cb;
    if ((cb = findBuiltinCmd (cmd->executable)))
        cb (cmd, g_env);

    printf("%sa\n", cmd->executable);

    switch (exec (cmd, envp))
    {
    case 1:
        fprintf (stderr, "%s: command not found.\n", cmd->executable);
        break;

    default:
        break;
    }
}

void *mainInput (void *vargp)
{
    char input [80];
    while (!feof (stdin))
    {
        printf ("352 %s# ", g_env->m_ptrWd);
        fflush (stdout);
        fgets (input, 80, stdin);

        array_t *a = B_parse_input_runs (input, 80);

        // Reject blank lines
        if (!a->length)
            continue;

        run_t *run       = ((run_t *)a->array [0]);
        cmd_t **commands = run->commands;
        unsigned c;
        for (c = 0; c < run->commands_size; c++)
        {
            command (commands [c], (char **)vargp);
        }
    }

    pthread_exit (0);
}

int main (int argc, char *argv [], char *envp [])
{
    init ();

    pthread_t g_inputThread;
    pthread_create (&g_inputThread, NULL, &mainInput, envp);

    while (1)
    {
        // User closed stdin (typically means we should exit)
        if (g_shellStatus & SHELL_DIE)
        {
#ifdef DEBUG
            printf ("D: SHELL_DIE caught\n");
#endif
            break;
        }
    }

    shutdown ();

    return 0;
}
