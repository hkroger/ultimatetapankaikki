#ifndef __TK___DRAW__
#define __TK___DRAW__
#include "CLASSES.H"
void draw_block( int x, int y, int type, int num, int mode );
void draw_dark_block( int x, int y, int num, int mode );
void draw_rambo( int x, int y, int angle, int num, int mode, int col );
void draw_shadow( int x, int y, int num, int mode );
void draw_sprite( int x, int y, int xs, int ys, char *spr, int xsize, int offs, int mode );
void draw_trans_sprite( int x, int y, int xs, int ys, char *spr, int xsize, int offs, int mode );
void draw_crate( int x, int y, int type, int mode );
void draw_enemy( int x, int y, int angle, int num, int mode, char *spr );
void draw_target( int x, int y, int mode );
void draw_effect( int x, int y, Effect_type *type, int num, int mode );
void draw_body_part( int x, int y, int num, int mode );
void draw_light( Light_table *lt, int x, int y, int num, int add, int mode );
void draw_ball( int x, int y, int num );
void draw_ware( int x, int y, int row, int colu );
void draw_black();
void draw_floor();
void draw_shadows();
void draw_walls();
int chk4draw( int x, int y, int sx, int sy, int size, int mode );
void draw_live_enemies();
void draw_players( int pos );
void draw_gae_shadows();
void draw_effects( int pos );
void draw_spots();
void draw_bullets();
void draw_body_parts();
void draw_infos();
void draw_energies();
void map();
void draw_box1( int x1, int y1, int x2, int y2, int add );
void rectangle( int x1, int y1, int x2, int y2, int col );
void draw_hits();
void draw_targets();
void draw_crates();
void draw_crate_texts();
void draw_kills();
#endif
