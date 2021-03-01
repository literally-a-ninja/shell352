#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "path.h"
#include "string.h"

void path_resolve_home(char *path, unsigned len)
{
    // Absolute directory
    if (*path == PATH_OPERATOR_DIRECTORY)
        return;

    // First directory is not expicitly "~" (E.g., ~this, ~that)
    char *firstDir = path_dir_first(path, len);
    if (strcmp(firstDir, PATH_OPERATOR_HOME))
        return;

    char *nextDir       = B_path_dir_next_ptr(path, len);
    unsigned nextLen    = B_strlen(nextDir);
    char *homeDir       = getenv("HOME");
    unsigned homeLen    = B_strlen(homeDir);

    unsigned blkSize  = sizeof(char) * homeLen + nextLen + (nextDir ? 1 : 0);
    path = realloc(path, blkSize);
    path = strncpy(path, homeDir, homeLen);

    if (nextDir) {
        path = strncpy(path + homeLen, "/", 1);
        path = strncpy(path + homeLen + 1, nextDir, nextLen);
    }
}


char *path_normalise(char *absPath, unsigned len)
{
    // We only deal with absolute paths, return immediately.
    if (*absPath != '/')
    {
        fprintf(stderr, "E: Attempted to normalise relative path (%s)\n", absPath);
        exit(0);
    }

    char **stack = malloc(sizeof(char*) * len);
    char **ptrStack = stack;

    char *c;
    unsigned l;
    for (
            c = absPath;
            *c;
            l = B_strlen(c), c = B_path_dir_next_ptr(c+1, l)
        )
    {
        char *dirName = path_dir_first(c, l);

        // Parent references move the stack ptr back one and clears the previous dirname.
        if (! strcmp(dirName, ".."))
        {
            // Too many parent references, bail and user will get the proper error.
            if (ptrStack == stack)
                return absPath;

            *--ptrStack = 0;
        }

        // Skips current references
        if (! strcmp(dirName, "."))                 continue;
        // Skips repeating "/"s which aren't root
        if (! B_strlen(dirName) && c != absPath)    continue;

        *ptrStack++ = dirName;
    }

    char **p;
    char *normalPath = malloc(sizeof(char));
    for (p = stack; ptrStack - p; p++)
    {
        normalPath = B_strcpy(normalPath, *p);
        normalPath = B_strcpy(normalPath, "/");
    }

    free(stack);

    return normalPath;
}

char *path_resolve_relative(
        char *relativePath,
        unsigned relativeLen,
        char *workDir,
        unsigned workLen
    )
{
    char *absPtr;
    char *absPath = malloc(sizeof(char) * PATH_MAX_DIR_LENGTH);
    char *relativePathCpy = malloc(sizeof(char) * relativeLen);
    relativePathCpy = B_strncpy(relativePathCpy, relativePath, relativeLen);


    path_resolve_home(relativePathCpy, relativeLen);

    // Great! We have an absolute path.
    if (*relativePathCpy == PATH_OPERATOR_DIRECTORY)
    {
        free(absPath);
        return relativePathCpy;
    }

    strncpy(absPath,                workDir,            workLen);
    strncpy(absPath + workLen,      "/",                1);
    strncpy(absPath + workLen + 1,  relativePathCpy,    relativeLen);

    // Resize proper span to avoid taking up too much space.
    unsigned absLen = B_strlen(absPath);

    absPath = realloc(absPath, sizeof(char) * (absLen + 1));
    char *normalPath = path_normalise(absPath, absLen);
    printf("%s\n", normalPath);
    free(relativePathCpy);

    return absPath;
}

char *path_resolve_dir_symb(char *dirName, unsigned dirLen)
{
    char *pwd = getenv("PWD");

    // Matches '' and '~'
    if (!dirName || strcmp(dirName, PATH_OPERATOR_HOME))
        return getenv("HOME");

    // Matches '.'
    else if (strcmp(dirName, PATH_OPERATOR_CURRENT_DIR))
        return path_dir_current(pwd, strlen(pwd));

    else 
        return dirName;
}

char *B_path_dir_last_ptr(char *path, unsigned len)
{
    char *ptr;

    unsigned offset = (len - 1);
    for(ptr = path + offset; ptr - path && *ptr == PATH_OPERATOR_DIRECTORY; ptr--);

    // We're at the root of existance, do nothing.
    if (ptr == path) return 0;

    return ptr;
}


char *B_path_dir_next_ptr(char *path, unsigned len)
{
    char *ptr;
    for(ptr = path; *ptr && *ptr == PATH_OPERATOR_DIRECTORY; ptr++);

    // We're at the root of existance, do nothing.
    if (ptr == path) return 0;

    return ptr;
}

char *path_dir_current(char *path, unsigned len)
{
    char* ptr = B_path_dir_last_ptr(path, len);

    unsigned dirLen = len - (path - ptr);
    char *currentDir = malloc(dirLen * sizeof(char));
    strncpy(currentDir, ptr + 1, dirLen);

    return currentDir;
}


char *path_dir_first(char *path, unsigned len)
{
    char* ptr = B_path_dir_next_ptr(path, len);
    unsigned dist = ptr - path;

    char *firstDir = malloc(sizeof(char) * dist);
    return strncpy(firstDir, path, dist);
}

char *path_dir_last(char *path, unsigned len)
{
    char* ptr = B_path_dir_last_ptr(path, len);

    // Calculate exactly what we need.
    unsigned dist = ptr - path;
    char *lastPath = malloc(sizeof(char) * dist);
    strncpy(lastPath, path, dist);

    return lastPath;
}

void path_mod_dir_last(char *path, unsigned len)
{
    char* ptr;

    if ((ptr = B_path_dir_last_ptr(path, len)))
        *ptr = 0;
}

struct path_list *B_path_str_to_list(char *path, unsigned len)
{
    unsigned i;
    char *ptr = path;
    unsigned arrAllocSize = 0;
    char **arr = malloc(sizeof(char*) * arrAllocSize);

    for (i = 0; i < len; i++)
    {
        // Either at EOS or found dir operator
        if (i + 1 == len || *(path + i) == PATH_OPERATOR_DIRECTORY)
        {
            unsigned length = 0
                + (/* dist from path start */ i)
                - (/* dir start - path start */ ptr - path)
                + (/* ptr offset by 1 */ 1);

            char *dirName = malloc(sizeof(char) * length);
            strncpy(dirName, ptr, length);

            arr[arrAllocSize] = dirName;

            ptr = path + i + 1;
            arr = realloc(arr, sizeof(char*) * ++arrAllocSize);
        }
    }

    struct path_list *list = malloc(sizeof(struct path_list));
    list->m_arrDirectories = arr;
    list->m_iDirectoryCount = arrAllocSize - 1;

    return list;
}

char *B_path_list_to_str(struct path_list *list)
{
    unsigned length = 0;
    char *path = malloc(sizeof(char) * 0);

    int i;
    for(i = 0; i < list->m_iDirectoryCount; i++)
    {
        char *dirName = list->m_arrDirectories[i];
        unsigned dirLen = B_strlen(dirName);

        length += dirLen + 1;
        path = realloc(path, sizeof(char) * length);
        strncpy(path, "/", 1);
        strncpy(path + 1, dirName, dirLen);
    }

    return path;
}
