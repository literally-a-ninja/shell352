#include <sys/types.h>

#define MAX_LINE 80
#define MAX_ARGS (MAX_LINE/2 + 1)
#define REDIRECT_OUT_OP '>'
#define REDIRECT_IN_OP '<'
#define PIPE_OP '|'
#define BG_OP '&'

/* Holds a single command. */
typedef struct Cmd {
  /* The command as input by the user. */
  char line[MAX_LINE + 1];
  /* The command as null terminated tokens. */
  char tokenLine[MAX_LINE + 1];
  /* Pointers to each argument in tokenLine, non-arguments are NULL. */
  char * args[MAX_ARGS];
  /* Pointers to each symbol in tokenLine, non-symbols are NULL. */
  char * symbols[MAX_ARGS];
  /* The process id of the executing command. */
  pid_t pid;
  /* TODO: Additional fields may be helpful. */
} Cmd;
