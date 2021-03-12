#ifndef H_EXEC
#define H_EXEC

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "buongiorno/common.h"
#include "buongiorno/file.h"

void applyRedirects (cmd_t *cmd);

int assertSpawn (int fdOut, int procId);

int exec_or_fail (cmd_t *cmd);

int spawn (run_t *run, unsigned depth, int out [2]);

int execute_runs (run_t *run);

#endif
