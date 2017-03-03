#ifndef _ORANGEPI_DEBUG_H_
#define _ORANGEPI_DEBUG_H_

void DEBUG_ORANGEPI(const char *, ...);
void DEBUG_ERROR(const char *, ...);

#define DEBUG_WARN DEBUG_ORANGEPI
#endif
