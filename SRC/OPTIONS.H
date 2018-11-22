#ifndef __TK___OPTIONS__
#define __TK___OPTIONS__

void save_options();
void load_options();
int choose_episode();
int choose_load_file( char *name, int episode );
void options_scr( int opt, char *filename );
void multip_options_scr( int opt, char *filename );
void player_options();
void game_options();
void sound_options();
int deathmatch_options();
void multiplayer_options();
void options();
#endif
