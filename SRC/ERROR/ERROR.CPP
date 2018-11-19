#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "KEYB.H"

void error(char *fmt,...)
{	auto va_list ap;

	// TODO VIDEO vesa_set_mode(0x3);

	va_start(ap,fmt);
	vprintf(fmt,ap);
	va_end(ap);

	exit(1);
}

