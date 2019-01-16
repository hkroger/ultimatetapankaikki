#ifndef __TK__MISCFUNC__
#define __TK__MISCFUNC__

#include <string>
#include <vector>
#include <SDL.h>
#include "INPUT/INPUT.H"
#include "TYPES.H"

void change_name( int num );
char *k_2_c( const i::Input& input );
i::Input get_key( int x, int y );
void clear_shit( int y );
void set_keys();
void alusta_client();
void alusta_players1();
void alusta_players2();
void two_to_one();
void define_bullet_types();
void define_effects();
void explo( int x, int y, int size, int index);
std::string data_path( const std::string& pathname = "" );
std::string config_path( const std::string& pathname = "" );
std::string home_path( const std::string& pathname = "" );
int ensure_config_dir_exists();
void list_files( int *no, vile *filelist, const std::string& wildcard, int type );
#define _A_SUBDIR 0
#define _A_NORMAL 1
#ifndef TK_PORT_MSVC
// windows.h has these already
char *strlwr( char *str );
char *strupr( char *str );
#endif
#endif
