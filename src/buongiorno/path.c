#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "path.h"
#include "string.h"

void path_resolve_home (char *path, unsigned len)
{
    // Absolute directory
    if (*path == PATH_OPERATOR_DIRECTORY)
        return;

    // First directory is not expicitly "~" (E.g., ~this, ~that)
    char *firstDir = path_dir_first (path, len);
    if (strcmp (firstDir, PATH_OPERATOR_HOME))
        return;

    char *nextDir = B_path_dir_next_ptr (path, len);
    char *homeDir = getenv ("HOME");

    if (!nextDir)
    {
        path = B_strcpy (path, homeDir);
        return;
    }

    unsigned homeLen = B_strlen (homeDir);
    unsigned nextLen = B_strlen (nextDir);
    unsigned blkSize = sizeof (char) * homeLen + nextLen + (nextDir ? 1 : 0);

    path = realloc (path, blkSize);
    path = strncpy (path, homeDir, homeLen);

    if (nextDir)
    {
        path = strncpy (path + homeLen, "/", 1);
        path = strncpy (path + homeLen + 1, nextDir, nextLen);
    }
}

void path_normalise (char *absPath, unsigned len)
{
    // We only deal with absolute paths, return immediately.
    if (*absPath != '/')
    {
        fprintf (stderr, "E: Attempted to normalise relative path (%s)\n",
                 absPath);
    }

    char *absPathCpy = malloc (sizeof (char) * len + 1);
    absPathCpy       = strncpy (absPathCpy, absPath, len);

    char **stack    = malloc (sizeof (char *) * len);
    char **ptrStack = stack;
    char *token;
    for (token = strtok (absPathCpy, "/"); token; token = strtok (NULL, "/"))
    {
        if (!strcmp (token, ".."))
        {
            ptrStack  = max (stack, ptrStack - 1);
            *ptrStack = 0;
            continue;
        }
        else if (!strcmp (token, "."))
        {
            continue;
        }

        *(ptrStack++) = B_strcpy (NULL, token);
    }
    *ptrStack = 0;

    char *normalPath = malloc (sizeof (char) * PATH_MAX_DIR_LENGTH);
    for (ptrStack = stack; *ptrStack; ptrStack++)
    {
        strncat (normalPath, "/", 2);
        strcat (normalPath, *ptrStack);
        free (*ptrStack);
    }

    absPath = B_strcpy (absPath, normalPath);
    free (stack);
    free (normalPath);
}

char *path_resolve_relative (char *relativePath, unsigned relativeLen,
                             char *workDir, unsigned workLen)
{
    char *absPath         = malloc (sizeof (char) * PATH_MAX_DIR_LENGTH);
    char *relativePathCpy = malloc (sizeof (char) * relativeLen);
    relativePathCpy = B_strncpy (relativePathCpy, relativePath, relativeLen);

    path_resolve_home (relativePathCpy, relativeLen);

    // Great! We have an absolute path.
    if (*relativePathCpy == PATH_OPERATOR_DIRECTORY)
    {
        free (absPath);
        return relativePathCpy;
    }

    strncpy (absPath, workDir, workLen);
    strncpy (absPath + workLen, "/", 1);
    strncpy (absPath + workLen + 1, relativePathCpy, relativeLen);

    free (relativePathCpy);

    // Resize proper span to avoid taking up too much space.
    unsigned absLen = B_strlen (absPath);
    path_normalise (absPath, absLen);

    return absPath;
}

char *path_resolve_dir_symb (char *dirName, unsigned dirLen)
{
    char *pwd = getenv ("PWD");

    // Matches '' and '~'
    if (!dirName || strcmp (dirName, PATH_OPERATOR_HOME))
        return getenv ("HOME");

    // Matches '.'
    else if (strcmp (dirName, PATH_OPERATOR_CURRENT_DIR))
        return path_dir_current (pwd, strlen (pwd));

    else
        return dirName;
}

char *path_dir_current (char *path, unsigned len)
{
    char *ptr = B_path_dir_last_ptr (path, len);

    unsigned dirLen  = len - (path - ptr);
    char *currentDir = malloc (dirLen * sizeof (char));
    strncpy (currentDir, ptr + 1, dirLen);

    return currentDir;
}

char *path_dir_first (char *path, unsigned len)
{
    char *ptr;

    if (!(ptr = B_path_dir_next_ptr (path, len)))
        return path;

    unsigned dist = ptr - path;

    char *firstDir = malloc (sizeof (char) * dist);
    return strncpy (firstDir, path, dist);
}

char *path_dir_last (char *path, unsigned len)
{
    char *ptr;
    if (!(ptr = B_path_dir_last_ptr (path, len)))
        return path;

    // Calculate exactly what we need.
    unsigned dist  = ptr - path;
    char *lastPath = malloc (sizeof (char) * dist);
    strncpy (lastPath, path, dist);

    return lastPath;
}

void path_mod_dir_last (char *path, unsigned len)
{
    char *ptr;

    if ((ptr = B_path_dir_last_ptr (path, len)))
        *ptr = 0;
}

char *B_path_dir_next_ptr (char *path, unsigned len)
{
    char *ptr;
    for (ptr = path; *ptr && *ptr == PATH_OPERATOR_DIRECTORY; ptr++)
        ;

    // We're at the root of existance, do nothing.
    if (ptr == path)
        return 0;

    return ptr;
}

char *B_path_list_to_str (struct path_list *list)
{
    unsigned length = 0;
    char *path      = malloc (sizeof (char) * 0);

    int i;
    for (i = 0; i < list->m_iDirectoryCount; i++)
    {
        char *dirName   = list->m_arrDirectories [i];
        unsigned dirLen = B_strlen (dirName);

        length += dirLen + 1;
        path = realloc (path, sizeof (char) * length);
        strncpy (path, "/", 1);
        strncpy (path + 1, dirName, dirLen);
    }

    return path;
}

char *B_path_dir_last_ptr (char *path, unsigned len)
{
    char *ptr;

    unsigned offset = (len - 1);
    for (ptr = path + offset; ptr - path && *ptr == PATH_OPERATOR_DIRECTORY;
         ptr--)
        ;

    // We're at the root of existance, do nothing.
    if (ptr == path)
        return 0;

    return ptr;
}
