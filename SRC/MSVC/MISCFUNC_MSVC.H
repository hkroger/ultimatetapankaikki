#ifndef __TK__MISCFUNC_MSVC__
#define __TK__MISCFUNC_MSVC__

#include "TYPES.H"

// windows.h and TK have define clashes so windows.h cannot be
// easily included from TK code. Therefore a separate implementation
// was created to encapsulate the windows.h include to separate
// source file.

void list_files_msvc( int *no, vile *filelist, const char *wildcard, int type );

#endif
