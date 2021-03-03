#include <stdlib.h>

#include "buongiorno/symbols.h"
#include "buongiorno/common.h"

#include "shell352.h"

int builtin_exit(cmd* cmd, struct environment *e) {
    g_shellStatus |= SHELL_DIE;
    return 0;
}
