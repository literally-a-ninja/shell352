#ifndef H_EXEC
#define H_EXEC

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "buongiorno/common.h"
#include "buongiorno/file.h"
#include "jobs.h"

#define RETURNVAL_FATAL_ERROR 0xDEAD

/**
 * CHILD
 *
 * Attaches appropriate redirects specified in cmd_t.
 **/
void exec_safe_attach_redirects (cmd_t *cmd);

/**
 * CHILD
 *
 * Responsible for the safe transition to execvp.
 **/
int exec_safe_execvp (cmd_t *cmd);

/**
 * CHILD
 *
 * Recursive spawning function in properly piping each command appropriately.
 **/
int exec_safe_spawn (job_t *job, unsigned depth, int out [2]);

/**
 * CHILD
 *
 * Handles actions resulting from returned inner-child values.
 **/
void exec_safe_handle_returnval (int *wstatus);

/**
 * PARENT
 *
 * Handles actions resulting from returned child values.
 **/
void exec_handle_returnval (int *wstatus);

int exec_job (job_t *job);

#endif
