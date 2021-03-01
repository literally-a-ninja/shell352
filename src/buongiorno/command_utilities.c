#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "symbols.h"
#include "command_utilities.h"

char *g_arrPathDirs[] = {
    "/usr/local/sbin", "/usr/local/bin", "/usr/sbin",
	"/usr/bin", "/sbin", "/bin"
};

/* Finds the index of the first occurance of symbol in cmd->symbols.
 * Returns -1 if not found. */
int findSymbol(Cmd * cmd, char symbol) {
	for (int i = 0; i < MAX_ARGS; i++) {
		char* commandSymbol = cmd->symbols[i];
		if (*commandSymbol == symbol)
			return i;
	}
	return -1;
}


/* Parses the command string contained in cmd->line.
 * Assumes all fields in cmd (except cmd->line) are initailized to zero.
 * On return, all fields of cmd are appropriatly populated. */
void parseCmd(Cmd * cmd) {
	char * token;
	int i = 0;
	strcpy(cmd -> tokenLine, cmd -> line);
	strtok(cmd -> tokenLine, "\n");
	token = strtok(cmd -> tokenLine, " ");

	while (token != NULL) {
		if ( * token == '\n') {
			cmd -> args[i] = NULL;
		} else if ( * token == REDIRECT_OUT_OP || * token == REDIRECT_IN_OP ||
			* token == PIPE_OP || * token == BG_OP) {
			cmd -> symbols[i] = token;
			cmd -> args[i] = NULL;
		} else {
			cmd -> args[i] = token;
		}

		token = strtok(NULL, " ");
		i++;
	}

	cmd -> args[i] = NULL;
}

int file_exists(char* fileName)
{
	return access(fileName, F_OK) != -1;
}

char* file_resolve(char* fileName)
{
	if (file_exists(fileName)) return fileName;

	unsigned index;
	char *fullPath = 0;
	for (index = 0; index < 6; index++)
	{
		if (!fullPath) 
			fullPath = (char*) malloc(64 * sizeof(char));

		sprintf(fullPath, "%s/%s", g_arrPathDirs[index], fileName);

		if (! file_exists(fullPath)) continue;

		return fullPath;
	}

	free(fullPath);
	return NULL;
}
