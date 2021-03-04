#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "file.h"

char *g_pathDirectories [FILE_PATH_DIRECTORY_LENGTH] = {
    "/usr/local/sbin", "/usr/local/bin", "/usr/sbin",
    "/usr/bin",        "/sbin",          "/bin"};

int file_exists (char *fileName)
{
    return access (fileName, F_OK) != -1;
}

char *file_resolve (char *fileName)
{
    if (file_exists (fileName))
        return fileName;

    char *fullPath;
    fullPath = (char *)malloc (64 * sizeof (char));
    unsigned index;
    for (index = 0; index < FILE_PATH_DIRECTORY_LENGTH; index++)
    {
        sprintf (fullPath, "%s/%s", g_pathDirectories [index], fileName);
        if (!file_exists (fullPath))
            continue;

        return fullPath;
    }

    free (fullPath);
    return NULL;
}
