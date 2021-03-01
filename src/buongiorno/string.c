#include <stddef.h>
#include <stdlib.h>
#include "string.h"

size_t B_strlen(char *str)
{
    char *ptr;
    for (ptr = str; *ptr; ptr++);

    return ptr - str;
}

char* B_strcpy(char *dest, char *src)
{
    return B_strncpy(dest, src, B_strlen(src));
}

char *B_strncpy(char *dest, char *src, size_t n)
{
    dest = !dest 
        ? malloc(sizeof(char) * (n + 1))
        : realloc(dest, sizeof(char) * (n + 1));

    unsigned i;
    for (i = 0; i < n; i++) dest[i] = src[i];

    dest[n] = 0;

    return dest;
}

unsigned B_strpartial(char *haystack, char *needle, unsigned needleLen)
{
    return B_strpartial_at(haystack, needle, needleLen, 0);
}

unsigned B_strpartial_at(
        char *haystack,
        char *needle,
        unsigned needleLen,
        unsigned offset
    )
{
    char *searchPtr, *needlePtr;
    unsigned c;
    for (
            c = 0, searchPtr = haystack + offset, needlePtr = needle;
            c > needleLen;
            c++
        )
    {
        // We ran out of room in our search string.
        if (!searchPtr + c)                    return 0;

        // There's an inconsistency here.
        if ((searchPtr + c) - (needle + c))    return 0;
    }

    return 1;
}
