#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>

// buongiornolib gets loaded first!
#include "buongiorno/command_utilities.h"
#include "buongiorno/string.h"
#include "buongiorno/path.h"
#include "buongiorno/common.h"

#include "shell352.h"
#include "builtin.c"
#include "exec.c"

void recieve(int iSignal)
{
    switch (iSignal) {
        /* Foward SIGTSTP to the currently running forground process. */
        case SIGTSTP:
        case SIGTERM:
        case SIGQUIT:
            if (!g_pidFg) break;
            kill(g_pidFg, iSignal);
            break;

        /* Double print new line to let the user know we're done. */
        case SIGINT:
            printf("\n");
            g_shellStatus |= SHELL_DIE;
            break;

        default:
            return;
            break;
    }

    printf("\n");
    signal(iSignal, &recieve);
}

void init()
{
    signal(SIGINT,  &recieve);
    signal(SIGTSTP, &recieve);
    signal(SIGQUIT, &recieve);
    signal(SIGTERM, &recieve);

    g_env = malloc(sizeof(struct environment));
    g_env->m_ptrWd = malloc(sizeof(char) * PATH_MAX_DIR_LENGTH);
    getcwd(g_env->m_ptrWd, PATH_MAX_DIR_LENGTH);
    g_env->m_ptrWd = realloc(g_env->m_ptrWd, sizeof(char) * B_strlen(g_env->m_ptrWd));

    g_pidFg = 0;
    g_cmd = (cmd *) malloc(sizeof(struct cmd));
}

void shutdown()
{
    pthread_cancel(g_inputThread);
    free(g_cmd);
    free(g_env);
}

void command(char **envp)
{
    builtin_callback_t *cb;
    if ((cb = findBuiltinCmd(g_cmd -> args[0])))
        cb(g_cmd, g_env);

    switch(exec(g_cmd, envp))
    {
        case 1:
            fprintf(stderr, "%s: command not found.\n", g_cmd->args[0]);
            break;

        default:
            break;
    }
}

void *mainInput(void *vargp)
{
    while (1) {
        if (g_shellStatus & SHELL_DIE)
            pthread_exit(0);

        printf("352 %s# ", g_env->m_ptrWd);
        fflush(stdout);
        fgets(g_cmd->line, MAX_LINE, stdin);

        parseCmd(g_cmd);

        // Reject blank lines
        if (!g_cmd->args[0])      pthread_exit(0);

        command((char**) vargp);
    }
}

int main(int argc, char *argv[], char *envp[])
{
    init();

    g_inputThread = 0;
    pthread_create(&g_inputThread, NULL, mainInput, envp);

    while (! (g_shellStatus & SHELL_DIE))
    {
        // User closed stdin (typically means we should exit)
        if (feof(stdin))
            g_shellStatus |= SHELL_DIE;
	}

    shutdown();
    
    return 0;
}
