#ifndef H_352
#define H_352

#define SHELL_DIE 1 << 0

#include "buongiorno/symbols.h"
#include <pthread.h>
#include <unistd.h>

/**
 * Global shell handlers
 **/

pid_t g_pidFg;
struct cmd *g_cmd;
struct environment *g_env;
int g_shellStatus;
pthread_t g_inputThread;

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
void command (char **envp);

/**
 * shell352 : Called on shell pre-shutdown
 **/
void shutdown ();

#endif
