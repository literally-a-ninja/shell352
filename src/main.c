#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include "symbols.h"

#ifndef H_BUILTIN
#include "builtin.h"
#endif

#include "builtin.c"

/* The process if of the currently executing forground command, or 0. */
pid_t forgroundPid = 0;

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

/* Finds the index of the first occurance of symbol in cmd->symbols.
 * Returns -1 if not found. */
int findSymbol(Cmd * cmd, char symbol) {
  for (int i = 0; i < MAX_ARGS; i++) {
    if (cmd -> symbols[i] && * cmd -> symbols[i] == symbol) {
      return i;
    }
  }
  return -1;
}

/* Signal handler for SIGTSTP (SIGnal - Terminal SToP),
 * which is caused by the user pressing control+z. */
void sigtstpHandler(int sig_num) {
  /* Reset handler to catch next SIGTSTP. */
  signal(SIGTSTP, sigtstpHandler);
  if (forgroundPid > 0) {
    /* Foward SIGTSTP to the currently running forground process. */
    kill(forgroundPid, SIGTSTP);
    /* TODO: Add foreground command to the list of jobs. */
  }
}



int main(void) {
    /* Listen for control+z (suspend process). */
    signal(SIGTSTP, sigtstpHandler);
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
      builtin_callback_t *cb = findBuiltinCmd(cmd -> args[0]);
      if (cb) {
          cb();
      }
      if (findSymbol(cmd, BG_OP) != -1) {
        /* TODO: Run command in background. */
      } else {
        /* TODO: Run command in foreground. */
      }
      /* TODO: Check on status of background processes. */
    }
}
