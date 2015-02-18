#ifndef ERROR_H_
#define ERROR_H_

#include <stdarg.h>
#include <stdio.h>

// this defines in debug mode a printf like function to output debug message
#ifdef _DEBUG
void debug(const char *fmt, ...);
#else
#define debug(a)
#endif

// this structure will allow returning a code a specific message, it might be
// more helpfull than returning only an int as error code
typedef struct {
    int code;
    const char* message;
} Status;

// avoid name collisition by prepending MARIO_
#define MARIO_STATUS_SUCCCESS 0
#define MARIO_STATUS_ERROR 1

// add more error code here...
//#define MARIO_STATUS_

#endif  // STATUS_H_
