#ifndef H_BUILTIN
#define H_BUILTIN

#include "buongiorno/builtin.h"
#define BUILTIN_CMDS_LENGTH 6

/**
 *  Returns function pointer on builtin cmd found.
 **/
builtin_callback_t *findBuiltinCmd (char *name);

#endif
