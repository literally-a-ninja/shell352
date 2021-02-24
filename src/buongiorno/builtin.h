#ifndef H_BUILTIN
#define H_BUILTIN

#define BUILTIN_CMD_NOT_FOUND 0
#define BUILTIN_CMDS_LENGTH 2

typedef int (builtin_callback_t)(); 

/**
 * Defines a basic built-in cmd data structure.
 **/
struct builtin_t
{
    char* name;
    builtin_callback_t* cb;
};

/**
 * Array holds all builtin command fn pointers
 **/
extern const struct builtin_t builtin_cmds[];

/**
 * Returns callback to builtin command if found, BUILTIN_CMD_NOT_FOUND if not found.
 **/
builtin_callback_t *findBuiltinCmd(char *name);

#endif
