#ifndef __TK__MISCFUNC__
#define __TK__MISCFUNC__

#include <SDL.h>
#include "INPUT.H"

void change_name( int num );
char *k_2_c( const tk_port::Input& scancode );
tk_port::Input get_key( int x, int y );
void clear_shit( int y );
void set_keys();
void alusta_client();
void alusta_players1();
void alusta_players2();
void two_to_one();
void define_bullet_types();
void define_effects();
void explo( int x, int y, int size, int index);
void list_files( int *no, vile *filelist, const char *wildcard, int type );
#define _A_SUBDIR 0
#define _A_NORMAL 1
#ifndef TK_PORT_MSVC
// windows.h has these already
char *strlwr( char *str );
char *strupr( char *str );
#endif
#endif
