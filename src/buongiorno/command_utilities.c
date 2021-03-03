#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "symbols.h"
#include "command_utilities.h"

int findSymbol(cmd * cmd, char symbol) {
	for (int i = 0; i < MAX_ARGS; i++) {
		char* commandSymbol = cmd->symbols[i];
		if (*commandSymbol == symbol)
			return i;
	}
	return -1;
}


void parseCmd(cmd * cmd)
{
	strcpy(cmd->tokenLine, cmd->line);
	strtok(cmd->tokenLine, "\n");

	char *token;
	token = strtok(cmd->tokenLine, " ");

    // TODO: Add quotation container support

    unsigned i;
    for(i = 0; token != NULL; i++)
    {
        switch(*token)
        {
            /* matches '>', '<', '|', '&' */
            case REDIRECT_OUT_OP:
            case REDIRECT_IN_OP:
            case PIPE_OP:
            case BG_OP:
                cmd->symbols[i] = token;

            /* matches '\n' */
            case CARRAIGE_RETURN:
                cmd->args[i] = NULL;
                break;

            default:
                cmd->args[i] = token;
                break;
		}

		token = strtok(NULL, " ");
	}

	cmd->args[i] = NULL;
}

