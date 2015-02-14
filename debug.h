#ifndef DEBUG_H_
#define DEBUG_H_

#include <stdarg.h>
#include <stdio.h>

#ifdef _DEBUG
void debug(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
}
#else
#define debug(a)
#endif

#endif  // DEBUG_H_
