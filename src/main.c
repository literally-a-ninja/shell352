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
#include "buongiorno/command.h"
#include "buongiorno/common.h"
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
        if (!g_pidFg) break;
        kill (g_pidFg, iSignal);
        break;

    /* Double print new line to let the user know we're done. */
    case SIGINT:
        if (g_pidFg)
            kill (g_pidFg, iSignal);
        else {
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
    g_cmd   = (cmd *)malloc (sizeof (struct cmd));
}

void shutdown ()
{
    free (g_cmd);
    free (g_env->m_ptrWd);
    free (g_env);

    pthread_join (g_inputThread, NULL);

#ifdef DEBUG
    printf (
        "D: shutdown() : Successfully freed all resources, returning to main() "
        "for termination.\n");
#endif
}

void command (char **envp)
{
    builtin_callback_t *cb;
    if ((cb = findBuiltinCmd (g_cmd->args [0])))
        cb (g_cmd, g_env);

    switch (exec (g_cmd, envp))
    {
    case 1:
        fprintf (stderr, "%s: command not found.\n", g_cmd->args [0]);
        break;

    default:
        break;
    }
}

void *mainInput (void *vargp)
{
    while (!feof (stdin))
    {
        printf ("352 %s# ", g_env->m_ptrWd);
        fflush (stdout);
        fgets (g_cmd->line, MAX_LINE, stdin);
        B_parseCmd (g_cmd);

        // Reject blank lines
        if (!g_cmd->args [0])
            continue;

        command ((char **)vargp);
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
