#include <OrangePiV4L2/OrangePi_Config.h>
#include <OrangePiV4L2/OrangePi_Debug.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/*
 * Parse a argment list of variable string.
 */
void DEBUG_ORANGEPI(const char *fmt, ...)
{
    if (OrangePi_Get_DEBUG()) {
        va_list ap;
        
        va_start(ap, fmt);
        vfprintf(stdout, fmt, ap);
        va_end(ap);    
    }    
}

/*
 * Parse a argment list of variable string.
 */
void DEBUG_ERROR(const char *fmt, ...)
{
    va_list ap;
        
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);    
}
