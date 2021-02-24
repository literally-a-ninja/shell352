#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>

#include "buongiorno/builtin.h"
#include "buongiorno/command_utilities.h"
#include "buongiorno/symbols.h"

#include "exec.c"

pid_t g_pidFg = 0;

/* Signal handler for SIGTSTP (SIGnal - Terminal SToP),
 * which is caused by the user pressing control+z. */
void signalHandler(int iSignal) {
    //  Reset handler to catch next SIGTSTP.
    signal(iSignal, &signalHandler);

    printf("\n");

    // Exit the shell
    if (iSignal == SIGINT)
    {
        // Print twice as this is good UX
        printf("\n");

        // Bye now!
        exit(0);
    }

    // Do nothing.
	if (g_pidFg == 0)           return;

    // Foward SIGTSTP to the currently running forground process.
    switch (iSignal) {
        case SIGTSTP:
        case SIGTERM:
        case SIGQUIT:
            kill(g_pidFg, iSignal);
            break;

        default:
            break;
    }

}

int main(int argc, char *argv[], char * envp[])
{
    signal(SIGINT, &signalHandler);
	signal(SIGTSTP, &signalHandler);
	signal(SIGQUIT, &signalHandler);
	signal(SIGTERM, &signalHandler);

	while (1) {
		printf("352> ");
		fflush(stdout);
		Cmd * cmd = (Cmd * ) calloc(1, sizeof(Cmd));
		fgets(cmd -> line, MAX_LINE, stdin);
		parseCmd(cmd);

		if (!cmd -> args[0]) {
		  	free(cmd);
		  	continue;
		}

		builtin_callback_t *cb;
		if ((cb = findBuiltinCmd(cmd -> args[0])))
            cb();

		if (exec(cmd, envp) < 0)
		{
            fprintf(stderr, "%s: command not found.\n", cmd->args[0]);
		}


		/* TODO: Check on status of background processes. */
	}

	return 0;
}
