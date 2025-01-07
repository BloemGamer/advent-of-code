//V0.2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdarg.h>

#include "../libraries/debug.h"

int debug_print(bool on,const char *str, ...)
{
	if(on)
    {
        va_list args;
        va_start(args, str);

        int result = vfprintf(stdout, str, args);

        va_end(args);
        return result;
    }
    else
	    return 0;
}