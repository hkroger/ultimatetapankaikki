#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cstdint>
#include "DEFINES.H"
#include "TYPES.H"
#include "GLOBVAR.H"
#include "CLASSES.H"
#include "WRITE.H"

 // 20 x 20
void draw_block( int x, int y, int type, int num, int mode )
{
	int a, offs;
	char *texture, *gofs;
	int sx = 0, ex = 20, sy = 0, ey = 20;
	if ( x < 0 ) sx =  - x;
	if ( sx > 20 ) sx = 20;
	if ( x > 300 ) ex = 20 - ( x - 300 );
	if ( ex < 0 ) ex = 0;
	if ( y < 0 ) sy =  - y;
	if ( sy > 20 ) sy = 20;
	if ( y > scr_y_size - 20 ) ey = 20 - ( y - ( scr_y_size - 20 )  );
	if ( ey < 0 ) ey = 0;
	if ( mode == 1 ) 
	{
		// split mode pl1
		if ( x > 140 ) ex = 20 - ( x - 140 );
		if ( ex < 0 ) ex = 0;
	}
	if ( mode == 2 ) 
	{
		// split mode pl2
		if ( x < 161 ) sx = 161 - x;
		if ( sx > 20 ) sx = 20;
	}
	offs = ( ( num&15 ) *20 )  + ( num / 16*320*20 )  + sy*320;
	gofs = virbuff + (( y + sy ) *320 + x + sx);
	if ( type == FLOOR ) texture = floorspr + offs + sx;
	else if ( type == WALLS ) texture = wallspr + offs + sx;
	else return;
	for ( a = sy; a < ey; a ++  ) 
	{
		memcpy( gofs, texture, ex - sx );
		texture += 320;
		gofs += 320;
	}
}

 // 20 x 20
void draw_dark_block( int x, int y, int num, int mode ) 
{
	int a, offs;
	char *gofs;

	int sx = 0, ex = 20, sy = 0, ey = 20;
	if ( x < 0 ) sx =  - x;
	if ( sx > 20 ) sx = 20;
	if ( x > 300 ) ex = 20 - ( x - 300 );
	if ( ex < 0 ) ex = 0;
	if ( y < 0 ) sy =  - y;
	if ( sy > 20 ) sy = 20;
	if ( y > scr_y_size - 20 ) ey = 20 - ( y - ( scr_y_size - 20 )  );
	if ( ey < 0 ) ey = 0;
	if ( mode == 1 ) 
	{
		// split mode pl1
		if ( x > 140 ) ex = 20 - ( x - 140 );
		if ( ex < 0 ) ex = 0;
	}
	if ( mode == 2 ) 
	{
		// split mode pl2
		if ( x < 161 ) sx = 161 - x;
		if ( sx > 20 ) sx = 20;
	}
	offs = ( ( num&15 ) *20 )  + ( num / 16*320*20 )  + sy*320;
	gofs = virbuff + (( y + sy ) *320 + x + sx);
	for ( a = sy; a < ey; a ++  ) 
	{
		memcpy( gofs, floorspr2 + offs + sx, ex - sx );
		offs += 320;
		gofs += 320;
	}
}

void draw_shadow( int x, int y, int num, int mode ) 
{
	int a, b, sx = 0, ex = 20, sy = 0, ey = 20;
	char *s, *vb;
	char colo;
	if ( x < 0 ) sx =  - x;
	if ( sx > 20 ) sx = 20;
	if ( x > 300 ) ex = 20 - ( x - 300 );
	if ( ex < 0 ) ex = 0;
	if ( y < 0 ) sy =  - y;
	if ( sy > 20 ) sy = 20;
	if ( y > scr_y_size - 20 ) ey = 20 - ( y - ( scr_y_size - 20 )  );
	if ( ey < 0 ) ey = 0;
	if ( mode == 1 ) 
	{
		// split mode pl1
		if ( x > 140 ) ex = 20 - ( x - ( 140 )  );
		if ( ex < 0 ) ex = 0;
	}
	if ( mode == 2 ) 
	{
		// split mode pl2
		if ( x < 161 ) sx = 161 - x;
		if ( sx > 20 ) sx = 20;
	}

	s = shadowspr + (( sy*320 )  + num*20);
	vb = virbuff + (( y + sy ) *320 + x);

	for ( a = sy; a < ey; a ++ , vb += 320, s += 320 ) 
	for ( b = sx; b < ex; b ++  ) 
	{
		colo = s[b];
		if ( colo!= 0 ) vb[b] = shadow_table[(uint8_t)vb[b]][(uint8_t)colo];
	}
}

 // 28 x 28
void draw_rambo( int x, int y, int angle, int num, int mode, int col ) 
{
	int a, b, c, d, offs1, offs2, sx = 0, ex = 28, sy = 0, ey = 28;
	int colo;
	if ( x < 0 ) sx =  - x;
	if ( sx > 28 ) sx = 28;
	if ( x > 292 ) ex = 28 - ( x - 292 );
	if ( ex < 0 ) ex = 0;
	if ( y < 0 ) sy =  - y;
	if ( sy > 28 ) sy = 28;
	if ( y > scr_y_size - 28 ) ey = 28 - ( y - ( scr_y_size - 28 )  );
	if ( ey < 0 ) ey = 0;
	if ( mode == 1 ) 
	{
		// split mode pl1
		if ( x > 132 ) ex = 28 - ( x - 132 );
		if ( ex < 0 ) ex = 0;
	}
	if ( mode == 2 ) 
	{
		// split mode pl2
		if ( x < 161 ) sx = 161 - x;
		if ( sx > 28 ) sx = 28;
	}
	offs1 = ( y + sy ) *320 + x;
	offs2 = ( ( sy + 1 + ( num*29 )  ) *1160 )  + ( angle*29 )  + 1;
	for ( a = sy, c = 0, d = 0; a < ey; a++, c += 320, d += 1160 )
	{
		for ( b = sx; b < ex; b++ )
		{
			colo = rambospr[offs2 + d + b];
			if ( colo )
			{
				if ( colo >= 80 && colo <= 80 + 15 )
				{
					virbuff[offs1 + c + b] = (char) (col + (colo - 80));
				}
				else
				{
					virbuff[offs1 + c + b] = (char) colo;
				}
			}
		}
	}
}

void draw_sprite( int x, int y, int xs, int ys, char *spr, int xsize, int offs, int mode ) 
{
	int a, b, offs1, offs2, sx = 0, ex = xs, sy = 0, ey = ys;
	char *src, *dest;
	char colo;
	if ( x < 0 ) sx =  - x;
	if ( sx > xs ) sx = xs;
	if ( x > 320 - xs ) ex = xs - ( x - ( 320 - xs )  );
	if ( ex < 0 ) ex = 0;
	if ( y < 0 ) sy =  - y;
	if ( sy > ys ) sy = ys;
	if ( y > scr_y_size - ys ) ey = ys - ( y - ( scr_y_size - ys )  );
	if ( ey < 0 ) ey = 0;
	if ( mode == 1 ) 
	{
		// split mode pl1
		if ( x > 160 - xs ) ex = xs - ( x - ( 160 - xs )  );
		if ( ex < 0 ) ex = 0;
	}
	if ( mode == 2 ) 
	{
		// split mode pl2
		if ( x < 161 ) sx = 161 - x;
		if ( sx > xs ) sx = xs;
	}
	offs1 = ( y + sy ) *320 + x;
	offs2 = ( sy*xsize )  + offs;
	src = spr + offs2 + sx;
	dest = virbuff + offs1 + sx;
	for ( a = sy; a < ey; a ++ , dest += 320 - ( ex - sx ), src += xsize - ( ex - sx )  ) 
	for ( b = sx; b < ex; b ++  ) 
	{
		colo = *( src ++  );
		if ( colo!= 0 ) ( *dest )  = colo;
		dest ++;
	}
}

void draw_trans_sprite( int x, int y, int xs, int ys, char *spr, int xsize, int offs, int mode ) 
{
	int a, b, offs1, offs2, sx = 0, ex = xs, sy = 0, ey = ys;
	char *src, *dest;
	char colo;
	if ( x < 0 ) sx =  - x;
	if ( sx > xs ) sx = xs;
	if ( x > 320 - xs ) ex = xs - ( x - ( 320 - xs )  );
	if ( ex < 0 ) ex = 0;
	if ( y < 0 ) sy =  - y;
	if ( sy > ys ) sy = ys;
	if ( y > scr_y_size - ys ) ey = ys - ( y - ( scr_y_size - ys )  );
	if ( ey < 0 ) ey = 0;
	if ( mode == 1 ) 
	{
		// split mode pl1
		if ( x > 160 - xs ) ex = xs - ( x - ( 160 - xs )  );
		if ( ex < 0 ) ex = 0;
	}
	if ( mode == 2 ) 
	{
		// split mode pl2
		if ( x < 161 ) sx = 161 - x;
		if ( sx > xs ) sx = xs;
	}
	offs1 = ( y + sy ) *320 + x;
	offs2 = ( sy*xsize )  + offs;
	src = spr + offs2 + sx;
	dest = virbuff + offs1 + sx;
	for ( a = sy; a < ey; a++, dest += 320 - (ex - sx), src += xsize - (ex - sx))
	{
		for ( b = sx; b < ex; b++ )
		{
			colo = *(src++);
			if ( colo != 0 )
				(*dest) = trans_table[(uint8_t) colo][(uint8_t) (*dest)];
			dest++;
		}
	}
}

void draw_crate( int x, int y, int type, int mode ) 
{
	draw_sprite( x, y, 14, 14, cratespr, 320, type*14, mode );
}

void draw_enemy( int x, int y, int angle, int num, int mode, char *spr ) 
{
	draw_sprite( x, y, 28, 28, spr, 1160, ( num*29*1160 )  + ( angle*29 )  + 1160 + 1, mode );
}

void draw_target( int x, int y, int mode ) 
{
	draw_sprite( x, y, 7, 7, targetspr, 7, 0, mode );
}

void draw_effect( int x, int y, Effect_type *type, int num, int mode ) 
{
	if ( type-> index!= FOOTPRINT &&type-> index!= SMOKE2 &&type-> index!= BLOOD2 &&type-> index!= SMOKE3 &&type-> index!= STEAM ) 
	draw_sprite( x - type-> xs / 2, y - type-> ys / 2, type-> xs, type-> ys, effectspr, 320, type-> offs + ( ( type-> xs + 1 ) *num )  + 321, mode );
	else 
	draw_trans_sprite( x - type-> xs / 2, y - type-> ys / 2, type-> xs, type-> ys, effectspr, 320, type-> offs + ( ( type-> xs + 1 ) *num )  + 321, mode );
}

void draw_body_part( int x, int y, int num, int mode ) 
{
	draw_sprite( x, y, 20, 20, bodypartspr, 320, ( num*21 ), mode );
}

void draw_light( Light_table *lt, int x, int y, int num, int add, int mode ) 
{
	int xs = Light_sizes[num], ys = Light_sizes[num];
	int a, b, c, sum, offs1, offs2, sx = 0, ex = xs, sy = 0, ey = ys;
	x -= ( xs / 2 );
	y -= ( ys / 2 );
	if ( x < 0 ) sx =  - x;
	if ( sx > xs ) sx = xs;
	if ( x > 320 - xs ) ex = xs - ( x - ( 320 - xs )  );
	if ( ex < 0 ) ex = 0;
	if ( y < 0 ) sy =  - y;
	if ( sy > ys ) sy = ys;
	if ( y > scr_y_size - ys ) ey = ys - ( y - ( scr_y_size - ys )  );
	if ( ey < 0 ) ey = 0;
	if ( mode == 1 ) 
	{
		// 2 pl mode pl1
		if ( x > 160 - xs ) ex = xs - ( x - ( 160 - xs )  );
		if ( ex < 0 ) ex = 0;
	}
	if ( mode == 2 ) 
	{
		// 2 pl mode pl2
		if ( x < 161 ) sx = 161 - x;
		if ( sx > xs ) sx = xs;
	}
	offs1 = ( y + sy ) *320 + x;
	for ( a = sy, offs2 = a*xs, c = 0; a < ey; a ++ , c += 320, offs2 += xs ) 
	for ( b = sx; b < ex; b ++  ) 
	if ( light[num].data[offs2 + b] ) 
	{
		sum = light[num].data[offs2 + b] + add;
		if ( sum < 0 ) sum = 0;
		if ( sum > 15 ) sum = 15;
		virbuff[offs1 + c + b] = lt-> data[(uint8_t)virbuff[offs1 + c + b]][(uint8_t)sum];
	}
}

void draw_ball( int x, int y, int num ) 
{
	int a, b, offs1, offs2;
	offs1 = ( ( num % 16 ) *20 )  + ( 59*320 )  + ( ( num / 16 ) *( 320*20 )  );
	offs2 = ( y*320 )  + x;
	for ( a = 0; a < 20; a++ )
		memcpy( virbuff + offs2 + (a * 320), picture + offs2 + (a * 320), 20 );
	for ( a = 0; a < 20; a++, offs1 += 300, offs2 += 300 )
		for ( b = 0; b < 20; b++, offs1++, offs2++ )
			if ( warespr[offs1] != 0 ) virbuff[offs2] = warespr[offs1];
	offs2 = (y * 320) + x;
	for ( a = 0; a < 20; a++ )
		memcpy( screen + offs2 + (a * 320), virbuff + offs2 + (a * 320), 20 );
}

void draw_ware( int x, int y, int row, int colu ) 
{
	int a, b, offs1, offs2;
	offs1 = row*320*17 + colu*17;
	offs2 = ( y*320 )  + x;
	for ( a = 0; a < 16; a++, offs1 += 304, offs2 += 304 )
		for ( b = 0; b < 16; b++, offs1++, offs2++ )
			if ( warespr[offs1] != 0 ) virbuff[offs2] = warespr[offs1];
			else virbuff[offs2] = picture[offs2];
}

void draw_black() 
{
	int level_size = level_x_size*level_y_size, a, b, ovx1, ovy1, ovy2, ovx2, offs;
	ovx1 = aplayer[0]->scr_x % 20;
	ovy1 = aplayer[0]->scr_y % 20;
	if ( GAME_MODE!= SPLIT_SCREEN ) 
	{
		offs = ( aplayer[0]->scr_y / 20 ) *level_x_size + ( aplayer[0]->scr_x / 20 );
		for ( a = 0; a < scr_y_size / 20 + 1; a ++ , offs += level_x_size - 17 ) 
		for ( b = 0; b < 17; b ++ , offs ++  ) 
		if ( offs < level_size ) 
		if ( level[offs].type == FLOOR ) 
		if ( !aplayer[0]->see_level[offs] ) 
		draw_dark_block( ( b*20 )  - ovx1, ( a*20 )  - ovy1, level[offs].num, 0 );
	}
	else
	{
		ovx2 = aplayer[1]->scr_x % 20;
		ovy2 = aplayer[1]->scr_y % 20;
		offs = ( aplayer[0]->scr_y / 20 ) *level_x_size + ( aplayer[0]->scr_x / 20 );
		for ( a = 0; a < scr_y_size / 20 + 1; a ++ , offs += level_x_size - 9 ) 
		for ( b = 0; b < 9; b ++ , offs ++  ) 
		if ( offs < level_size ) 
		if ( level[offs].type == FLOOR ) 
		if ( !aplayer[0]->see_level[offs] ) 
		draw_dark_block( ( b*20 )  - ovx1, ( a*20 )  - ovy1, level[offs].num, 1 );
		offs = ( aplayer[1]->scr_y / 20 ) *level_x_size + ( aplayer[1]->scr_x / 20 );
		for ( a = 0; a < scr_y_size / 20 + 1; a ++ , offs += level_x_size - 9 ) 
		for ( b = 0; b < 9; b ++ , offs ++  ) 
		if ( offs < level_size ) 
		if ( level[offs].type == FLOOR ) 
		if ( !aplayer[1]->see_level[offs] ) 
		draw_dark_block( ( b*20 )  - ovx2 + 160, ( a*20 )  - ovy2, level[offs].num, 2 );
	}
}

void draw_floor() 
{
	int a, b, do_it, ovx1, ovy1, ovy2, ovx2, offs;
	int level_size = level_x_size*level_y_size;
	ovx1 = aplayer[0]->scr_x % 20;
	ovy1 = aplayer[0]->scr_y % 20;
	if ( GAME_MODE!= SPLIT_SCREEN ) 
	{
		offs = ( aplayer[0]->scr_y / 20 ) *level_x_size + ( aplayer[0]->scr_x / 20 );
		for ( a = 0; a < scr_y_size / 20 + 1; a ++ , offs += level_x_size - 17 ) 
		for ( b = 0; b < 17; b ++ , offs ++  ) 
		if ( offs < level_size ) 
		if ( level[offs].type == FLOOR ) 
		{
			do_it = 1;
			if ( DARK_MODE ) if ( !aplayer[0]->see_level[offs] ) do_it = 0;
			if ( do_it ) 
			draw_block( ( b*20 )  - ovx1, ( a*20 )  - ovy1, level[offs].type, level[offs].num, 0 );
		}
	}
	else
	{
		ovx2 = aplayer[1]->scr_x % 20;
		ovy2 = aplayer[1]->scr_y % 20;
		offs = ( aplayer[0]->scr_y / 20 ) *level_x_size + ( aplayer[0]->scr_x / 20 );
		for ( a = 0; a < scr_y_size / 20 + 1; a ++ , offs += level_x_size - 9 ) 
		for ( b = 0; b < 9; b ++ , offs ++  ) 
		if ( offs < level_size ) 
		if ( level[offs].type == FLOOR ) 
		{
			do_it = 1;
			if ( DARK_MODE ) if ( !aplayer[0]->see_level[offs] ) do_it = 0;
			if ( do_it ) 
			draw_block( ( b*20 )  - ovx1, ( a*20 )  - ovy1, level[offs].type, level[offs].num, 1 );
		}
		offs = ( aplayer[1]->scr_y / 20 ) *level_x_size + ( aplayer[1]->scr_x / 20 );
		for ( a = 0; a < scr_y_size / 20 + 1; a ++ , offs += level_x_size - 9 ) 
		for ( b = 0; b < 9; b ++ , offs ++  ) 
		if ( offs < level_size ) 
		if ( level[offs].type == FLOOR ) 
		{
			do_it = 1;
			if ( DARK_MODE ) if ( !aplayer[1]->see_level[offs] ) do_it = 0;
			if ( do_it ) 
			draw_block( ( b*20 )  - ovx2 + 160, ( a*20 )  - ovy2, level[offs].type, level[offs].num, 2 );
		}
	}
}

void draw_shadows() 
{
	int a, b, ovx1, ovy1, ovy2, ovx2, offs;
	int dou, level_size = level_x_size*level_y_size;
	ovx1 = aplayer[0]->scr_x % 20;
	ovy1 = aplayer[0]->scr_y % 20;
	if ( GAME_MODE!= SPLIT_SCREEN ) 
	{
		offs = ( aplayer[0]->scr_y / 20 ) *level_x_size + ( aplayer[0]->scr_x / 20 );
		for ( a = 0; a < scr_y_size / 20 + 1; a ++ , offs += level_x_size - 17 ) 
		for ( b = 0; b < 17; b ++ , offs ++  ) 
		if ( offs < level_size ) 
		if ( level[offs].shadow > 0 ) 
		{
			dou = 1;
			if ( level[offs].type == FLOOR ) if ( DARK_MODE ) if ( !aplayer[0]->see_level[offs] ) dou = 0;
			if ( dou ) draw_shadow( ( b*20 )  - ovx1, ( a*20 )  - ovy1, level[offs].shadow - 1, 0 );
		}
	}
	else
	{
		ovx2 = aplayer[1]->scr_x % 20;
		ovy2 = aplayer[1]->scr_y % 20;
		offs = ( aplayer[0]->scr_y / 20 ) *level_x_size + ( aplayer[0]->scr_x / 20 );
		for ( a = 0; a < scr_y_size / 20 + 1; a ++ , offs += level_x_size - 9 ) 
		for ( b = 0; b < 9; b ++ , offs ++  ) 
		if ( offs < level_size ) 
		if ( level[offs].shadow > 0 ) 
		{
			dou = 1;
			if ( level[offs].type == FLOOR ) if ( DARK_MODE ) if ( !aplayer[0]->see_level[offs] ) dou = 0;
			if ( dou ) draw_shadow( ( b*20 )  - ovx1, ( a*20 )  - ovy1, level[offs].shadow - 1, 1 );
		}
		offs = ( aplayer[1]->scr_y / 20 ) *level_x_size + ( aplayer[1]->scr_x / 20 );
		for ( a = 0; a < scr_y_size / 20 + 1; a ++ , offs += level_x_size - 9 ) 
		for ( b = 0; b < 9; b ++ , offs ++  ) 
		if ( offs < level_size ) 
		if ( level[offs].shadow > 0 ) 
		{
			dou = 1;
			if ( level[offs].type == FLOOR ) if ( DARK_MODE ) if ( !aplayer[1]->see_level[offs] ) dou = 0;
			if ( dou ) draw_shadow( ( b*20 )  - ovx2 + 160, ( a*20 )  - ovy2, level[offs].shadow - 1, 2 );
		}
	}
}

void draw_walls() 
{
	int a, b, ovx1, ovy1, ovy2, ovx2, offs;
	int level_size = level_x_size*level_y_size;
	ovx1 = aplayer[0]->scr_x % 20;
	ovy1 = aplayer[0]->scr_y % 20;
	if ( GAME_MODE!= SPLIT_SCREEN ) 
	{
		offs = ( aplayer[0]->scr_y / 20 ) *level_x_size + ( aplayer[0]->scr_x / 20 );
		for ( a = 0; a < scr_y_size / 20 + 1; a ++ , offs += level_x_size - 17 ) 
		for ( b = 0; b < 17; b ++ , offs ++  ) 
		if ( offs < level_size ) 
		{
			if ( level[offs].type == WALLS ) 
			draw_block( ( b*20 )  - ovx1, ( a*20 )  - ovy1, level[offs].type, level[offs].num, 0 );
		}
		else draw_block( ( b*20 )  - ovx1, ( a*20 )  - ovy1, WALLS, 19, 0 );
	}
	else
	{
		ovx2 = aplayer[1]->scr_x % 20;
		ovy2 = aplayer[1]->scr_y % 20;
		offs = ( aplayer[0]->scr_y / 20 ) *level_x_size + ( aplayer[0]->scr_x / 20 );
		for ( a = 0; a < scr_y_size / 20 + 1; a ++ , offs += level_x_size - 9 ) 
		for ( b = 0; b < 9; b ++ , offs ++  ) 
		if ( offs < level_size ) 
		{
			if ( level[offs].type == WALLS ) 
			draw_block( ( b*20 )  - ovx1, ( a*20 )  - ovy1, level[offs].type, level[offs].num, 1 );
		}
		else draw_block( ( b*20 )  - ovx1, ( a*20 )  - ovy1, WALLS, 19, 1 );
		offs = ( aplayer[1]->scr_y / 20 ) *level_x_size + ( aplayer[1]->scr_x / 20 );
		for ( a = 0; a < scr_y_size / 20 + 1; a ++ , offs += level_x_size - 9 ) 
		for ( b = 0; b < 9; b ++ , offs ++  ) 
		if ( offs < level_size ) 
		{
			if ( level[offs].type == WALLS ) 
			draw_block( ( b*20 )  - ovx2 + 160, ( a*20 )  - ovy2, level[offs].type, level[offs].num, 2 );
		}
		else draw_block( ( b*20 )  - ovx2 + 160, ( a*20 )  - ovy2, WALLS, 19, 2 );
	}
	if ( GAME_MODE == SPLIT_SCREEN ) 
	for ( a = 160; a < 320*scr_y_size; a+=320) 
	virbuff[a] = 0;
}

int chk4draw( int x, int y, int sx, int sy, int size, int mode ) 
{
	if ( mode == 0 &&
	     x > sx - size &&
	     x < sx + 320 &&
	     y > sy - size &&
	     y < sy + scr_y_size )
		return (1);
	if ( mode > 0 &&
	     x > sx - size &&
	     x < sx + 160 &&
	     y > sy - size &&
	     y < sy + scr_y_size )
		return (1);
	return (0);
}

void draw_live_enemies() 
{
	int a;
	for ( a = 0; a < ENEMIES; a ++  ) 
	{
		enemy[a].rangle = enemy[a].angle / 9;
		if ( GAME_MODE!= SPLIT_SCREEN ) 
		if ( !enemy[a].DEAD ) 
		if ( chk4draw( enemy[a].x, enemy[a].y, aplayer[0]->scr_x, aplayer[0]->scr_y, 28, 0 )  ) 
		draw_enemy( enemy[a].x - aplayer[0]->scr_x, enemy[a].y - aplayer[0]->scr_y, enemy[a].rangle, enemy[a].ANIM, 0, enemy_info[enemy[a].type].spr );
		if ( GAME_MODE == SPLIT_SCREEN ) 
		if ( !enemy[a].DEAD ) 
		{
			if ( chk4draw( enemy[a].x, enemy[a].y, aplayer[0]->scr_x, aplayer[0]->scr_y, 28, 1 )  ) 
			draw_enemy( enemy[a].x - aplayer[0]->scr_x, enemy[a].y - aplayer[0]->scr_y, enemy[a].rangle, enemy[a].ANIM, 1, enemy_info[enemy[a].type].spr );
			if ( chk4draw( enemy[a].x, enemy[a].y, aplayer[1]->scr_x, aplayer[1]->scr_y, 28, 1 )  ) 
			{
				draw_enemy( ( enemy[a].x - aplayer[1]->scr_x )  + 160, enemy[a].y - aplayer[1]->scr_y, enemy[a].rangle, enemy[a].ANIM, 2, enemy_info[enemy[a].type].spr );
			}
		}
	}
}

void draw_players( int pos ) 
{
	int a, draw1 = 0;
	if ( GAME_MODE!= SPLIT_SCREEN ) 
	for ( a = 0; a < MAX_PLAYERS; a ++  ) 
	if (player[a].enabled)
	{
		draw1 = 0;
		if ( pos == 0 ) if ( player[a].DEAD ) draw1 = 1;
		if ( pos == 1 ) if ( !player[a].DEAD ) draw1 = 1;
		if ( draw1 ) 
		if ( chk4draw( player[a].x, player[a].y, aplayer[0]->scr_x, aplayer[0]->scr_y, 28, 0 )  ) 
		draw_rambo( player[a].x - aplayer[0]->scr_x, player[a].y - aplayer[0]->scr_y, player[a].rangle, player[a].ANIM, 0, player[a].color );
	}
	if ( GAME_MODE == SPLIT_SCREEN ) 
	{
		for ( a = 0; a < MAX_PLAYERS; a ++  ) 
    	if (player[a].enabled)
		{
			draw1 = 0;
			if ( pos == 0 ) if ( player[a].DEAD ) draw1 = 1;
			if ( pos == 1 ) if ( !player[a].DEAD ) draw1 = 1;
			if ( draw1 ) 
			{
				if ( chk4draw( player[a].x, player[a].y, aplayer[0]->scr_x, aplayer[0]->scr_y, 28, 1 )  ) 
				draw_rambo( player[a].x - aplayer[0]->scr_x, player[a].y - aplayer[0]->scr_y, player[a].rangle, player[a].ANIM, 1, player[a].color );
				if ( chk4draw( player[a].x, player[a].y, aplayer[1]->scr_x, aplayer[1]->scr_y, 28, 2 )  ) 
				draw_rambo( player[a].x - aplayer[1]->scr_x + 160, player[a].y - aplayer[1]->scr_y, player[a].rangle, player[a].ANIM, 2, player[a].color );
			}
		}
	}
}

void draw_gae_shadows() 
{
	int a, xo =  - 1, yo = 7;
	for ( a = 0; a < ENEMIES; a ++  ) 
	{
		if ( GAME_MODE!= SPLIT_SCREEN ) 
		if ( !enemy[a].DEAD ) 
		if ( chk4draw( enemy[a].x, enemy[a].y, aplayer[0]->scr_x, aplayer[0]->scr_y, 28, 0 )  ) 
		draw_shadow( enemy[a].x - aplayer[0]->scr_x + xo, enemy[a].y - aplayer[0]->scr_y + yo, 5, 0 );
		if ( GAME_MODE == SPLIT_SCREEN ) 
		if ( !enemy[a].DEAD ) 
		{
			if ( chk4draw( enemy[a].x, enemy[a].y, aplayer[0]->scr_x, aplayer[0]->scr_y, 28, 1 )  ) 
			draw_shadow( enemy[a].x - aplayer[0]->scr_x + xo, enemy[a].y - aplayer[0]->scr_y + yo, 5, 1 );
			if ( chk4draw( enemy[a].x, enemy[a].y, player[1].scr_x, player[1].scr_y, 28, 1 )  ) 
			draw_shadow( enemy[a].x - aplayer[1]->scr_x + 160 + xo, enemy[a].y - aplayer[1]->scr_y + yo, 5, 2 );
		}
	}
	if ( GAME_MODE!= SPLIT_SCREEN ) 
	for ( a = 0; a < MAX_PLAYERS; a ++  ) 
	if (player[a].enabled)
	if ( !player[a].DEAD ) 
	if ( chk4draw( player[a].x, player[a].y, aplayer[0]->scr_x, aplayer[0]->scr_y, 28, 0 )  ) 
	draw_shadow( player[a].x - aplayer[0]->scr_x + xo, player[a].y - aplayer[0]->scr_y + yo, 5, 0 );
	if ( GAME_MODE == SPLIT_SCREEN ) 
	{
		for ( a = 0; a < MAX_PLAYERS; a ++  ) 
    	if (player[a].enabled)
		if ( !player[a].DEAD ) 
		if ( chk4draw( player[a].x, player[a].y, aplayer[0]->scr_x, aplayer[0]->scr_y, 28, 1 )  ) 
		draw_shadow( player[a].x - aplayer[0]->scr_x + xo, player[a].y - aplayer[0]->scr_y + yo, 5, 1 );
		for ( a = 0; a < MAX_PLAYERS; a ++  ) 
    	if (player[a].enabled)
		if ( !player[a].DEAD ) 
		if ( chk4draw( player[a].x, player[a].y, aplayer[1]->scr_x, aplayer[1]->scr_y, 28, 2 )  ) 
		draw_shadow( player[a].x - aplayer[1]->scr_x + 160 + xo, player[a].y - aplayer[1]->scr_y + yo, 5, 2 );
	}
}

void draw_effects( int pos ) 
{
	int a;
	for ( a = 0; a < MAX_EFFECTS; a ++  ) 
	if ( effect[a].IN_USE ) 
	if ( effect[a].type-> order == pos ) 
	{
		if ( GAME_MODE!= SPLIT_SCREEN ) 
		{
			if ( chk4draw( effect[a].x - ( effect[a].type-> xs / 2 ), effect[a].y - ( effect[a].type-> ys / 2 ), aplayer[0]->scr_x, aplayer[0]->scr_y, 25, 0 )  ) 
			switch( effect[a].type-> index ) 
			{
				case EXPLO:draw_light( effect[a].lt, effect[a].x - aplayer[0]->scr_x, effect[a].y - aplayer[0]->scr_y, 7, 5 - effect[a].count*0.8, 0 ); break;
				case METAL:if (LIGHT_EFFECTS) draw_light( effect[a].lt, effect[a].x - aplayer[0]->scr_x, effect[a].y - aplayer[0]->scr_y, 0,  - effect[a].count, 0 ); break;
				case SMOKE:if (LIGHT_EFFECTS) draw_light( effect[a].lt, effect[a].x - aplayer[0]->scr_x, effect[a].y - aplayer[0]->scr_y, 1,  - effect[a].count, 0 ); break;
				case TELEPORT:if (LIGHT_EFFECTS) draw_light( effect[a].lt, effect[a].x - aplayer[0]->scr_x, effect[a].y - aplayer[0]->scr_y, 4, 0, 0 ); break;
			}
			if (effect[a].type-> index != EXPLO) 
			 draw_effect( effect[a].x - aplayer[0]->scr_x, effect[a].y - aplayer[0]->scr_y, effect[a].type, effect[a].count / effect[a].type-> delay, 0 );
		}
		if ( GAME_MODE == SPLIT_SCREEN ) 
		{
			if ( chk4draw( effect[a].x - ( effect[a].type-> xs / 2 ), effect[a].y - ( effect[a].type-> ys / 2 ), aplayer[0]->scr_x, aplayer[0]->scr_y, 25, 1 )  ) 
			{
				if ( LIGHT_EFFECTS ) 
				switch( effect[a].type-> index ) 
				{
					case EXPLO:draw_light( effect[a].lt, effect[a].x - aplayer[0]->scr_x, effect[a].y - aplayer[0]->scr_y, 7, 5 - effect[a].count*0.8, 1 ); break;
					case METAL:if (LIGHT_EFFECTS) draw_light( effect[a].lt, effect[a].x - aplayer[0]->scr_x, effect[a].y - aplayer[0]->scr_y, 0,  - effect[a].count, 1 ); break;
					case SMOKE:if (LIGHT_EFFECTS) draw_light( effect[a].lt, effect[a].x - aplayer[0]->scr_x, effect[a].y - aplayer[0]->scr_y, 1,  - effect[a].count, 1 ); break;
					case TELEPORT:if (LIGHT_EFFECTS) draw_light( effect[a].lt, effect[a].x - aplayer[0]->scr_x, effect[a].y - aplayer[0]->scr_y, 4, 0, 1 ); break;
				}
			if (effect[a].type-> index != EXPLO) 
				 draw_effect( effect[a].x - aplayer[0]->scr_x, effect[a].y - aplayer[0]->scr_y, effect[a].type, effect[a].count / effect[a].type-> delay, 1 );
			}
			if ( chk4draw( effect[a].x - ( effect[a].type-> xs / 2 ), effect[a].y - ( effect[a].type-> ys / 2 ), aplayer[1]->scr_x, aplayer[1]->scr_y, 25, 1 )  ) 
			{
				if ( LIGHT_EFFECTS ) 
				switch( effect[a].type-> index ) 
				{
					case EXPLO:draw_light( effect[a].lt, effect[a].x - aplayer[1]->scr_x + 160, effect[a].y - aplayer[1]->scr_y, 7, 5 - effect[a].count*0.8, 2 ); break;
					case METAL:if (LIGHT_EFFECTS) draw_light( effect[a].lt, effect[a].x - aplayer[1]->scr_x + 160, effect[a].y - aplayer[1]->scr_y, 0,  - effect[a].count, 2 ); break;
					case SMOKE:if (LIGHT_EFFECTS) draw_light( effect[a].lt, effect[a].x - aplayer[1]->scr_x + 160, effect[a].y - aplayer[1]->scr_y, 1,  - effect[a].count, 2 ); break;
					case TELEPORT:if (LIGHT_EFFECTS) draw_light( effect[a].lt, effect[a].x - aplayer[1]->scr_x + 160, effect[a].y - aplayer[1]->scr_y, 4, 0, 2 ); break;
				}
			if (effect[a].type-> index != EXPLO) 
				 draw_effect( effect[a].x - aplayer[1]->scr_x + 160, effect[a].y - aplayer[1]->scr_y, effect[a].type, effect[a].count / effect[a].type-> delay, 2 );
			}
		}
	}
}

void draw_spots() 
{
	int a;
	for ( a = 0; a < Spot_amount; a ++  ) 
	{
		if ( GAME_MODE != SPLIT_SCREEN )
		{
			if ( chk4draw( spot_light[a].x - 30, spot_light[a].y - 30, aplayer[0]->scr_x, aplayer[0]->scr_y, 60, 0 ))
				draw_light( &normal_l, spot_light[a].x - aplayer[0]->scr_x, spot_light[a].y - aplayer[0]->scr_y,
				            spot_light[a].size, 2.5 * sini[Spot_count] + 2, 0 );
		}
		else
		{
			if ( chk4draw( spot_light[a].x - 30, spot_light[a].y - 30, aplayer[0]->scr_x, aplayer[0]->scr_y, 60, 1 ))
				draw_light( &normal_l, spot_light[a].x - aplayer[0]->scr_x, spot_light[a].y - aplayer[0]->scr_y,
				            spot_light[a].size, 2.5 * sini[Spot_count] + 2, 1 );
			if ( chk4draw( spot_light[a].x - 30, spot_light[a].y - 30, aplayer[1]->scr_x, aplayer[1]->scr_y, 60, 1 ))
				draw_light( &normal_l, spot_light[a].x - aplayer[1]->scr_x + 160, spot_light[a].y - aplayer[1]->scr_y,
				            spot_light[a].size, 2.5 * sini[Spot_count] + 2, 2 );
		}
	}
}

void draw_bullets() 
{
	int a;
	for ( a = 0; a < MAX_BULLETS; a ++  ) 
	if ( bullet[a].IN_USE ) 
	{
		if ( GAME_MODE!= SPLIT_SCREEN ) 
		if ( chk4draw( bullet[a].x, bullet[a].y, aplayer[0]->scr_x, aplayer[0]->scr_y, 20, 0 )  ) 
		{
			if ( bullet[a].type-> light != NULL ) 
			draw_light( bullet[a].type-> light, bullet[a].x - aplayer[0]->scr_x, bullet[a].y - aplayer[0]->scr_y, bullet[a].type-> lsize, bullet[a].type-> llum + bullet[a].count*bullet[a].type-> ladd, 0 );
			if ( bullet[a].type-> sprite != NULL ) 
			draw_sprite( bullet[a].x - ( int ) ( bullet[a].type-> xs >> 1 )  - aplayer[0]->scr_x, bullet[a].y - ( int ) ( bullet[a].type-> ys >> 1 )  - aplayer[0]->scr_y, bullet[a].type-> xs, bullet[a].type-> ys, bullet[a].type-> sprite, bullet[a].type-> xsize, bullet[a].type-> offs, 0 );
		}
		if ( GAME_MODE == SPLIT_SCREEN ) 
		{
			if ( chk4draw( bullet[a].x, bullet[a].y, aplayer[0]->scr_x, aplayer[0]->scr_y, 20, 1 )  ) 
			{
				if ( bullet[a].type-> light != NULL ) 
				draw_light( bullet[a].type-> light, bullet[a].x - aplayer[0]->scr_x, bullet[a].y - aplayer[0]->scr_y, bullet[a].type-> lsize, bullet[a].type-> llum + bullet[a].count*bullet[a].type-> ladd, 1 );
				if ( bullet[a].type-> sprite != NULL ) 
				draw_sprite( bullet[a].x - ( int ) ( bullet[a].type-> xs >> 1 )  - aplayer[0]->scr_x, bullet[a].y - ( int ) ( bullet[a].type-> ys >> 1 )  - aplayer[0]->scr_y, bullet[a].type-> xs, bullet[a].type-> ys, bullet[a].type-> sprite, bullet[a].type-> xsize, bullet[a].type-> offs, 1 );
			}
			if ( chk4draw( bullet[a].x, bullet[a].y, aplayer[1]->scr_x, aplayer[1]->scr_y, 20, 1 )  ) 
			{
				if ( bullet[a].type-> light != NULL ) 
				draw_light( bullet[a].type-> light, bullet[a].x - aplayer[1]->scr_x + 160, bullet[a].y - aplayer[1]->scr_y, bullet[a].type-> lsize, bullet[a].type-> llum + bullet[a].count*bullet[a].type-> ladd, 2 );
				if ( bullet[a].type-> sprite != NULL ) 
				draw_sprite( bullet[a].x - ( int ) ( bullet[a].type-> xs >> 1 )  - aplayer[1]->scr_x + 160, bullet[a].y - ( int ) ( bullet[a].type-> ys >> 1 )  - aplayer[1]->scr_y, bullet[a].type-> xs, bullet[a].type-> ys, bullet[a].type-> sprite, bullet[a].type-> xsize, bullet[a].type-> offs, 2 );
			}
		}
	}
}

void draw_body_parts() 
{
	int a;
	for ( a = 0; a < MAX_BODY_PARTS; a ++  ) 
	if ( body_part[a].IN_USE ) 
	{
		if ( GAME_MODE!= SPLIT_SCREEN ) 
		if ( chk4draw( body_part[a].x, body_part[a].y, aplayer[0]->scr_x, aplayer[0]->scr_y, 20, 0 )  ) 
		draw_body_part( body_part[a].x - aplayer[0]->scr_x, body_part[a].y - aplayer[0]->scr_y, body_part[a].num, 0 );
		if ( GAME_MODE == SPLIT_SCREEN ) 
		{
			if ( chk4draw( body_part[a].x, body_part[a].y, aplayer[0]->scr_x, aplayer[0]->scr_y, 20, 1 )  ) 
			draw_body_part( body_part[a].x - aplayer[0]->scr_x, body_part[a].y - aplayer[0]->scr_y, body_part[a].num, 1 );
			if ( chk4draw( body_part[a].x, body_part[a].y, aplayer[1]->scr_x, aplayer[1]->scr_y, 20, 1 )  ) 
			draw_body_part( body_part[a].x - aplayer[1]->scr_x + 160, body_part[a].y - aplayer[1]->scr_y, body_part[a].num, 2 );
		}
	}
}

void draw_infos() 
{
	char text[20];
	int x;
	writefonts( 3, scr_y_size - 15, weapon[aplayer[0]->curr_weapon].name, 1 );
	x = aplayer[0]->bullets[weapon[aplayer[0]->curr_weapon].btype - 1] / bullet_type[weapon[aplayer[0]->curr_weapon].btype].mul;
	if ( x < 0 ) x = 1;
	sprintf( text, "%d", x );
	if ( aplayer[0]->curr_weapon > 0 ) writefonts( 3, scr_y_size - 23, text, 7*16 );
	if ( GAME_MODE == SPLIT_SCREEN ) 
	{
		writefonts( 3 + 160, scr_y_size - 15, weapon[aplayer[1]->curr_weapon].name, 1 );
		x = aplayer[1]->bullets[weapon[aplayer[1]->curr_weapon].btype - 1] / bullet_type[weapon[aplayer[1]->curr_weapon].btype].mul;
		if ( x < 0 ) x = 1;
		sprintf( text, "%d", x );
		if ( aplayer[1]->curr_weapon > 0 ) writefonts( 3 + 160, scr_y_size - 23, text, 7*16 );
	}
}

void draw_energies() 
{
	int a, offs = ( ( scr_y_size - 5 ) *320 )  + 3;
	float div = ( ( float ) ( aplayer[0]->max_energy + ( aplayer[0]->shield*10 )  ) / 5 )  / 15;
	for ( a = 0; a < aplayer[0]->energy / 5; a ++  ) 
	{
		virbuff[offs + a] = 239 - ( a / div );
		virbuff[offs + a + 320] = 239 - ( a / div );
	}
	if ( GAME_MODE == SPLIT_SCREEN ) 
	{
		offs = ( ( scr_y_size - 5 ) *320 )  + 3 + 160;
		div = ( ( float ) ( aplayer[1]->max_energy + ( aplayer[1]->shield*10 )  ) / 5 )  / 15;
		for ( a = 0; a < aplayer[1]->energy / 5; a ++  ) 
		{
			virbuff[offs + a] = 239 - ( a / div );
			virbuff[offs + a + 320] = 239 - ( a / div );
		}
	}
}

void map() 
{
	int a, b, offs, cx = 0, cy = 0, type;
	if ( GAME_MODE!= SPLIT_SCREEN ) 
	{
		cx = 320 - ( level_x_size );
		cy = 0;
	}
	if ( GAME_MODE == SPLIT_SCREEN ) 
	{
		cx = 160 - ( level_x_size / 2 );
		cy = ( scr_y_size / 2 )  - ( level_y_size / 2 );
	}
	offs = cy*320 + cx;
	for ( a = 0; a < level_y_size; a ++ , offs += ( 320 - level_x_size )  ) 
	for ( b = 0; b < level_x_size; b ++ , offs ++  ) 
	{
		type = level[a*level_x_size + b].type;
		if ( type == WALLS ) virbuff[offs] = 16;
		else virbuff[offs] = shadow_table[(uint8_t)virbuff[offs]][15];
	}
	for ( a = 0; a < ENEMIES; a ++  ) 
	if ( enemy[a].DEAD ) 
	virbuff[( ( int ) ( ( enemy[a].y + 15 )  / 20 )  + cy ) *320 + ( int ) ( ( enemy[a].x + 15 )  / 20 )  + cx] = 9;
	if ( SHOW_ENEMIES ) 
	for ( a = 0; a < ENEMIES; a ++  ) 
	if ( !enemy[a].DEAD ) 
	virbuff[( ( int ) ( ( enemy[a].y + 15 )  / 20 )  + cy ) *320 + ( int ) ( ( enemy[a].x + 15 )  / 20 )  + cx] = 3;
	if (GAME_MODE!=SPLIT_SCREEN)
	for ( a = 0; a < MAX_PLAYERS; a ++  ) 
	if (KILLING_MODE!=DEATHMATCH || aplayer[0]->tindex==a)
	if (player[a].enabled)
	virbuff[( ( int ) ( ( player[a].y + 15 )  / 20 )  + cy ) *320 + ( int ) ( ( player[a].x + 15 )  / 20 )  + cx] = ( char ) player[a].color;
}

void draw_box1( int x1, int y1, int x2, int y2, int add ) 
{
	int a, b, c, offs = ( y1*320 )  + x1;
	for ( a = 0; a < ( y2 - y1 ); a ++ , offs += ( 320 - ( x2 - x1 )  )  ) 
	for ( b = 0; b < ( x2 - x1 ); b ++ , offs ++  ) 
	{
		if ( a == 0 ||a == y2 - y1 - 1 ||b == 0 ||b == x2 - x1 - 1 ) 
		c = picture[offs] + 1;
		else c = picture[offs] - add;
		if ( c < 0 ) c = 0;
		if ( c > 63 ) c = 63;
		picture[offs] = ( char ) c;
	}
}

void rectangle( int x1, int y1, int x2, int y2, int col ) 
{
	int a, offs, offs1, offs2, offs3;
	offs1 = y1*320 + x1;
	offs2 = y1*320 + x2;
	offs3 = y2*320 + x1;
	memset( virbuff + offs1, col, ( x2 - x1 )  + 1 );
	memset( virbuff + offs3, col, ( x2 - x1 )  + 1 );
	for( a = 0, offs = offs1 + 320; a < ( y2 - y1 )  - 1; a ++ , offs += 320 ) 
	virbuff[offs] = ( char ) col;
	for( a = 0, offs = offs2 + 320; a < ( y2 - y1 )  - 1; a ++ , offs += 320 ) 
	virbuff[offs] = ( char ) col;
}

void draw_hits() 
{
	if ( GAME_MODE!= SPLIT_SCREEN ) 
	if ( aplayer[0]->HIT > 0 ) 
	rectangle( 0, 0, 319, scr_y_size - 1, ( ( 4*16 )  - 1 )  - aplayer[0]->HIT );
	if ( GAME_MODE == SPLIT_SCREEN ) 
	{
		if ( aplayer[0]->HIT > 0 ) 
		rectangle( 0, 0, 159, scr_y_size - 1, ( ( 4*16 )  - 1 )  - aplayer[0]->HIT );
		if ( aplayer[1]->HIT > 0 ) 
		rectangle( 161, 0, 319, scr_y_size - 1, ( ( 4*16 )  - 1 )  - aplayer[1]->HIT );
	}
}

void draw_targets() 
{
	int a, dist;
	float x, y;
	static int plot_count = 0;
	plot_count ++;
	if ( GAME_MODE!= SPLIT_SCREEN ) 
	if ( !aplayer[0]->DEAD ) 
	if ( weapon[aplayer[0]->curr_weapon].gun ) 
	if ( aplayer[0]->TARGET ) 
	{
		x = aplayer[0]->x + 14 + 10*sini[aplayer[0]->rangle2];
		y = aplayer[0]->y + 14 + 10*cosi[aplayer[0]->rangle2];
		dist = bullet_type[weapon[aplayer[0]->curr_weapon].btype].time*bullet_type[weapon[aplayer[0]->curr_weapon].btype].speed;
		for ( a = 0; a < dist &&x - aplayer[0]->scr_x >= 0 &&x - aplayer[0]->scr_x < 320 &&y - aplayer[0]->scr_y >= 0 &&y - aplayer[0]->scr_y < scr_y_size &&level[( int ) ( y / 20 ) *level_x_size + ( int ) ( x / 20 ) ].type == FLOOR; a ++  ) 
		{
			if ( ( a - ( int ) ( plot_count / 2 )  )  % 6 == 0 ) 
			virbuff[( int ) ( y - aplayer[0]->scr_y ) *320 + ( int ) ( x - aplayer[0]->scr_x ) ] = 9*16;
			x += sini[aplayer[0]->rangle2];
			y += cosi[aplayer[0]->rangle2];
		}
		if ( a == dist ||level[( int ) ( y / 20 ) *level_x_size + ( int ) ( x / 20 ) ].type == WALLS ) draw_target( x - 3 - aplayer[0]->scr_x, y - 3 - aplayer[0]->scr_y, 0 );
	}
	if ( GAME_MODE == SPLIT_SCREEN ) 
	{
		if ( !aplayer[0]->DEAD ) 
		if ( weapon[aplayer[0]->curr_weapon].gun ) 
		if ( aplayer[0]->TARGET ) 
		{
			x = aplayer[0]->x + 14 + 10*sini[aplayer[0]->rangle2];
			y = aplayer[0]->y + 14 + 10*cosi[aplayer[0]->rangle2];
			dist = bullet_type[weapon[aplayer[0]->curr_weapon].btype].time*bullet_type[weapon[aplayer[0]->curr_weapon].btype].speed;
			for ( a = 0; a < dist &&x - aplayer[0]->scr_x >= 0 &&x - aplayer[0]->scr_x < 160 &&y - aplayer[0]->scr_y >= 0 &&y - aplayer[0]->scr_y < scr_y_size &&level[( int ) ( y / 20 ) *level_x_size + ( int ) ( x / 20 ) ].type == FLOOR; a ++  ) 
			{
				if ( ( a - ( int ) ( plot_count / 2 )  )  % 6 == 0 ) 
				virbuff[( int ) ( y - aplayer[0]->scr_y ) *320 + ( int ) ( x - aplayer[0]->scr_x ) ] = 9*16;
				x += sini[aplayer[0]->rangle2];
				y += cosi[aplayer[0]->rangle2];
			}
			if ( a == dist ||level[( int ) ( y / 20 ) *level_x_size + ( int ) ( x / 20 ) ].type == WALLS ) draw_target( x - 3 - aplayer[0]->scr_x, y - 3 - aplayer[0]->scr_y, 1 );
		}
		if ( !aplayer[1]->DEAD ) 
		if ( weapon[aplayer[1]->curr_weapon].gun ) 
		if ( aplayer[1]->TARGET ) 
		{
			dist = bullet_type[weapon[aplayer[1]->curr_weapon].btype].time*bullet_type[weapon[aplayer[1]->curr_weapon].btype].speed;
			x = aplayer[1]->x + 14 + 10*sini[aplayer[1]->rangle2];
			y = aplayer[1]->y + 14 + 10*cosi[aplayer[1]->rangle2];
			for ( a = 0; a < dist &&x - aplayer[1]->scr_x > 0 &&x - aplayer[1]->scr_x < 160 &&y - aplayer[1]->scr_y >= 0 &&y - aplayer[1]->scr_y < scr_y_size &&level[( int ) ( y / 20 ) *level_x_size + ( int ) ( x / 20 ) ].type == FLOOR; a ++  ) 
			{
				if ( ( a - ( int ) ( plot_count / 2 )  )  % 6 == 0 ) 
				virbuff[( int ) ( y - aplayer[1]->scr_y ) *320 + ( int ) ( x - aplayer[1]->scr_x )  + 160] = 9*16;
				x += sini[aplayer[1]->rangle2];
				y += cosi[aplayer[1]->rangle2];
			}
			if ( a == dist ||level[( int ) ( y / 20 ) *level_x_size + ( int ) ( x / 20 ) ].type == WALLS ) draw_target( x - 3 - aplayer[1]->scr_x + 160, y - 3 - aplayer[1]->scr_y, 2 );
		}
	}
}

void draw_crates() 
{
	int a;
	for ( a = 0; a < MAX_CRATES; a ++  ) 
	if ( crate[a].IN_USE ) 
	if ( crate[a].text_count == 0 ) 
	{
		if ( GAME_MODE!= SPLIT_SCREEN ) 
		if ( chk4draw( crate[a].x, crate[a].y, aplayer[0]->scr_x, aplayer[0]->scr_y, 14, 0 )  ) 
		{
			if ( LIGHT_EFFECTS ) draw_light( &normal_l, crate[a].x - aplayer[0]->scr_x + 7, crate[a].y - aplayer[0]->scr_y + 7, 4, 2, 0 );
			draw_crate( crate[a].x - aplayer[0]->scr_x, crate[a].y - aplayer[0]->scr_y, crate[a].type, 0 );
		}
		if ( GAME_MODE == SPLIT_SCREEN ) 
		{
			if ( chk4draw( crate[a].x, crate[a].y, aplayer[0]->scr_x, aplayer[0]->scr_y, 14, 1 )  ) 
			{
				if ( LIGHT_EFFECTS ) draw_light( &normal_l, crate[a].x - aplayer[0]->scr_x + 7, crate[a].y - aplayer[0]->scr_y + 7, 4, 2, 1 );
				draw_crate( crate[a].x - aplayer[0]->scr_x, crate[a].y - aplayer[0]->scr_y, crate[a].type, 1 );
			}
			if ( chk4draw( crate[a].x, crate[a].y, aplayer[1]->scr_x, aplayer[1]->scr_y, 14, 1 )  ) 
			{
				if ( LIGHT_EFFECTS ) draw_light( &normal_l, crate[a].x - aplayer[1]->scr_x + 7 + 160, crate[a].y - aplayer[1]->scr_y + 7, 4, 2, 2 );
				draw_crate( crate[a].x - aplayer[1]->scr_x + 160, crate[a].y - aplayer[1]->scr_y, crate[a].type, 2 );
			}
		}
	}
}

void draw_crate_texts() 
{
	int a;
	for ( a = 0; a < MAX_CRATES; a ++  ) 
	if ( crate[a].IN_USE ) 
	if ( crate[a].text_count > 0 ) 
	writefonts( crate[a].text_x, crate[a].text_y, crate[a].text, 32 );
}

void draw_kills() 
{
	char text[5], text2[20];
	int a,b=0;
	if ( GAME_MODE == SPLIT_SCREEN ) 
	{
		sprintf( text, "%d", aplayer[0]->player_kills );
		writefonts( 3, 3, text, 48 );
		sprintf( text, "%d", aplayer[1]->player_kills );
		writefonts( 317 - str_length( text ), 3, text, 144 );
	}
	else 
	for ( a = 0; a < MAX_PLAYERS;a++) 
	if (player[a].enabled)
	{
		strcpy( text2, player[a].name );
		sprintf( text, "%d", player[a].player_kills );
		strcat( text2, " " );
		strcat( text2, text );
		writefonts( 3 + ( b % 4 ) *80, 3 + ( b / 4 ) *10, text2, player[a].color );
		b++;
	}
}

