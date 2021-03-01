#include <stdlib.h>
#include "buongiorno/symbols.h"
#include "buongiorno/common.h"

int builtin_exit(Cmd* cmd, struct environment *e) {
    exit(0);
    return 0;
}
