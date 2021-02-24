#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "buongiorno/symbols.h"
#include "buongiorno/command_utilities.h"

int exec(Cmd* cmd, char* envp[])
{
	char* fileName = cmd->args[0];
	char* fullPath;

	// Doesn't exist, exit with -1 status
	if (! (fullPath = file_resolve(fileName)))
		return -1;

	int procId;
	if ((procId = fork()) < 0) return 1;

	// Parent
	if (procId)
	{
        g_pidFg = procId;
		wait(NULL);
	}

	// Child
	else 
	{
		execve(fullPath, cmd->args, envp);
	}

    free(fullPath);

	// if (findSymbol(cmd, BG_OP) != -1) {
	// 	/* TODO: Run command in background. */
	// } else {
	// 	/* TODO: Run command in foreground. */
	// }

    return 0;
}
