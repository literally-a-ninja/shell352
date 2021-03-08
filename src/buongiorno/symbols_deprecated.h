#ifndef HB_SYMBOLS_DEPRECATED
#define HB_SYMBOLS_DEPRECATED

#include <sys/types.h>

#define MAX_LINE 80
#define MAX_ARGS (MAX_LINE / 2 + 1)

/* Holds a single command. */
typedef struct cmd
{
    /* The command as input by the user. */
    char line [MAX_LINE + 1];
    /* The command as null terminated tokens. */
    char tokenLine [MAX_LINE + 1];
    /* Pointers to each argument in tokenLine, non-arguments are NULL. */
    char *args [MAX_ARGS];
    /* Pointers to each symbol in tokenLine, non-symbols are NULL. */
    char *symbols [MAX_ARGS];
    /* The process id of the executing command. */
    pid_t pid;
} cmd;

/* Holds a single command. */
typedef struct job_t
{
    /* The command as input by the user. */
    char line [MAX_LINE + 1];
    /* The command as null terminated tokens. */
    char tokenLine [MAX_LINE + 1];
    /* Pointers to each argument in tokenLine, non-arguments are NULL. */
    char *args [MAX_ARGS];
    /* Pointers to each symbol in tokenLine, non-symbols are NULL. */
    char *symbols [MAX_ARGS];
    /* The process id of the executing command. */
    pid_t pid;
    /* TODO: Additional fields may be helpful. */
} job_t;

#endif
