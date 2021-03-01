#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "buongiorno/symbols.h"
#include "buongiorno/path.h"
#include "buongiorno/string.h"
#include "buongiorno/common.h"

#define BUILTIN_CD_FALLBACK_DIR "~"

char *builtin_cd_get_target_dir(char **args) {
    if (args[1])
        return args[1];

    char *fallback = malloc(sizeof(char) * 2);
    fallback = B_strncpy(fallback, BUILTIN_CD_FALLBACK_DIR, 1);

    return fallback;
}

int builtin_cd(Cmd* cmd, struct environment *e) {
    char *pwd = getenv("PWD");
    char *targetDir = builtin_cd_get_target_dir(cmd->args);
    char *absPath = path_resolve_relative(
        targetDir,
        B_strlen(targetDir),
        pwd,
        B_strlen(pwd)
    );

    e->m_ptrWd = B_strcpy(e->m_ptrWd, absPath);
    chdir(e->m_ptrWd);
    free(absPath);

    return 0;
}
