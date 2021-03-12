#ifndef H_352
#define H_352

#define SHELL_DIE 1 << 0

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#include "buongiorno/grammar.h"

#define IREAD 0
#define IWRITE 1

/**
 * Global shell handlers
 **/

pid_t g_pidFg;
struct run_t *g_run;
struct environment *g_env;
int g_shellStatus;
pthread_t g_inputThread;

/* The process if of the currently executing forground command, or 0. */
extern pid_t g_pidFg;

/**
 * Lifecycle callbacks
 **/

/**
 * shell352 : Called on shell init
 **/
void init ();

/**
 * shell352 : Called on signal capture.
 **/
void recieve (int iSignal);

/**
 * shell352 : Called on command acceptance
 **/
void command (cmd_t *cmd, char **envp);

/**
 * shell352 : Called on shell pre-shutdown
 **/
void shutdown ();

#endif
