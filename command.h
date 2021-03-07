#ifndef HB_CMD_UTILS
#define HB_CMD_UTILS

#include "symbols.h"

/* Finds the index of the first occurance of symbol in cmd->symbols.
 * Returns -1 if not found. */
int findSymbol (cmd *cmd, char symbol);

/* Parses the command string contained in cmd->line.
 * Assumes all fields in cmd (except cmd->line) are initailized to zero.
 * On return, all fields of cmd are appropriatly populated. */
void parseCmd (cmd *cmd);

/**
 * Returns 1 if absolute path exists, 0 if missing.
 **/
int file_exists (char *fileName);

// I know, it's ugly. This is temporary until we can parse PATH correctly.
extern char *g_arrPathDirs [];

/**
 * Resolve a filename given PATH directories, returns malloc'd full path on
 * successful resolve. Otherwise, returns null if file not found.
 *
 **/
char *file_resolve (char *fileName);

#endif
