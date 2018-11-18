#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <vesa.h>
#include <keyb.hpp>

void error(char *fmt,...)
{	auto va_list ap;
	extern KeyboardHandler *CURRENTHANDLER;
	
	vesa_set_mode(0x3);
	if (CURRENTHANDLER!=NULL) CURRENTHANDLER->deinit();
	
	va_start(ap,fmt);
	vprintf(fmt,ap);
	va_end(ap);

	exit(1);
}

