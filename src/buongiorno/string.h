#ifndef HB_STRING
#define HB_STRING

#include <stddef.h>

/**
 * Returns string length of dynamically allocated string.
 **/
size_t B_strlen(char *str);

/**
 * Copies from src to dest with auto resize
 **/
char *B_strcpy(char *dest, char *src);


/**
 * Copies from src to dest with auto resize
 **/
char *B_strncpy(char *dest, char *src, size_t n);

/**
 * Returns true if haystack contains needle substring at start
 **/
unsigned B_strpartial(char *haystack, char *needle, unsigned needleLen);

/**
 * Returns true if haystack contains needle substring at offset
 **/
unsigned B_strpartial_at(char *haystack, char *needle, unsigned needleLen, unsigned offset);


#endif
