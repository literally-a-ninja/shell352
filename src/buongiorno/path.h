#ifndef HB_PATH
#define HB_PATH

#define PATH_MAX_DIR_LENGTH 1024
#define PATH_OPERATOR_HOME "~"
#define PATH_OPERATOR_CURRENT_DIR "."
#define PATH_OPERATOR_DIRECTORY '/'

struct path_list {
    char **m_arrDirectories;
    unsigned m_iDirectoryCount;
};

/**
 * Given common UNIX director symbols, returns the absolute resolved version.
 *
 * Handles:
 *  ~
 *  .
 **/
char *path_resolve_dir_symb(char *dirName, unsigned dirLen);

/**
 * Resolves relative path using provided work directory.
 **/
char *path_resolve(
        struct path_list *relativePaths,
        struct path_list *workingPaths
    );

/**
 * Properly resolves both parent current directory references.
 **/
char *path_normalise(
        char *path,
        unsigned len
    );

/**
 * Resolves relative path using provided work directory.
 **/
char *path_resolve_relative(char *relativePath, unsigned relativeLen, char *workDir, unsigned workLen);


/**
 * Cleans up a directory name by removing relative artifacts.
 **/
char *B_path_dirname_clean(char *path, unsigned len);

/**
 * Given a file path, returns the current directory
 **/
char *path_dir_current(char *path, unsigned len);

/**
 * Given a file path, returns the first directory
 **/
char *path_dir_first(char *path, unsigned len);

/**
 * Given a file path, returns one level above.
 **/
char *path_dir_last(char *path, unsigned len);

/**
 * Given a file path string, modifies the path to push back one.
 **/
void path_mod_dir_last(char *path, unsigned len);

/**
 * Locates the nearest directory operator from last and returns ptr.
 *
 * 0, if at root.
 **/
char *B_path_dir_last_ptr(char *path, unsigned len);

/**
 * Locates the nearest directory operator from start and returns ptr.
 *
 * 0, if at root.
 **/
char *B_path_dir_next_ptr(char *path, unsigned len);

/**
 * Converts a string to a path list.
 **/
struct path_list *B_path_str_to_list(char *path, unsigned len);

/**
 * Converts a path list to a string.
 **/
char *B_path_list_to_str(struct path_list *list);

#endif
