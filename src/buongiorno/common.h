#ifndef HB_COMMON
#define HB_COMMON

#include "symbols.h"

#define min(x, y) ((x < y) ? x : y)
#define max(x, y) ((x < y) ? y : x)

struct environment
{
    char *m_ptrWd;
};

#endif
