#ifndef HB_CMD_UTILS
#define HB_CMD_UTILS

#include "common.h"

/* Finds the index of the first occurance of symbol in cmd->symbols.
 * Returns -1 if not found. */
int B_findSymbol (cmd *cmd, char symbol);

/* Parses the command string contained in cmd->line.
 * Assumes all fields in cmd (except cmd->line) are initailized to zero.
 * On return, all fields of cmd are appropriatly populated. */
void B_parseCmd (cmd *cmd);

#endif
