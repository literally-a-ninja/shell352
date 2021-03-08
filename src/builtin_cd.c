#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "buongiorno/common.h"
#include "buongiorno/path.h"
#include "buongiorno/string.h"

#define BUILTIN_CD_FALLBACK_DIR "~"

char *builtin_cd_get_target_dir (char **args)
{
    if (args [1])
        return args [1];

    char *fallback = malloc (sizeof (char) * 2);
    fallback       = B_strncpy (fallback, BUILTIN_CD_FALLBACK_DIR, 1);

    return fallback;
}

int builtin_cd (cmd_t *cmd, struct environment *e)
{
    char *targetDir = builtin_cd_get_target_dir (cmd->args);
    char *absPath   = path_resolve_relative (targetDir, B_strlen (targetDir),
                                           e->m_ptrWd, B_strlen (e->m_ptrWd));

    struct stat stats;
    stat (absPath, &stats);

    if (!S_ISDIR (stats.st_mode))
    {
        fprintf (stderr, "cd: The directory \"%s\" does not exist.\n",
                 targetDir);
#ifdef DEBUG
        fprintf (stderr, "D: \"%s\".\n", absPath);
#endif
        free (absPath);
        return 1;
    }

    e->m_ptrWd = B_strcpy (e->m_ptrWd, absPath);
    chdir (e->m_ptrWd);
    free (absPath);

    return 0;
}
