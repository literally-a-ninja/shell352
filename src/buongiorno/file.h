#ifndef HB_FILE
#define HB_FILE

#define FILE_PATH_DIRECTORY_LENGTH 6

/**
 * Search directories
 **/
extern char *g_pathDirectories [];

/**
 * Returns 1 if file exists
 **/
int file_exists (char *fileName);

/**
 * Returns absolute path to file
 **/
char *file_resolve (char *fileName);

#endif
