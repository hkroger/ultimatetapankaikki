#ifndef __TK___WRITE__
#define __TK___WRITE__
void pixeli( int x, int y, char col );
void loadfonts( const char *name );
void writefonts( int x, int y, const char *str, int cq );
void writefonts2( int x, int y, const char *str, int cq );
int str_length( const char *str );
void readline( int x, int y, int len, char *str, char *screen );
#endif
