#define H_BUILTIN
#define BUILTIN_CMD_NOT_FOUND 0

typedef int (builtin_callback_t)(); 

struct builtin_t
{
    char* name;
    builtin_callback_t* cb;
};

/**
 * Returns callback to builtin command if found, BUILTIN_CMD_NOT_FOUND if not found.
 **/
builtin_callback_t *findBuiltinCmd(char *name);
