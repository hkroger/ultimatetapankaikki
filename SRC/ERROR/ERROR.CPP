#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "../KEYB.H"

void error(const char *fmt,...)
{
    va_list ap;

	va_start(ap,fmt);
	vprintf(fmt,ap);
	va_end(ap);

	exit(1);
}

void warn(const char *fmt,...)
{
	va_list ap;

	va_start(ap,fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
}
