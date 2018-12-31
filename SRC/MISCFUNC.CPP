#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <ctime>  // must be included before local headers due to clock() shenanigans
#include "KEYB.H"
#include "EFP/EFP.H"
#include "DRAW.H"
#include "GLOBVAR.H"
#include "FADE.H"
#include "WRITE.H"
#include "NEWFUNCS.H"
#include "ROUT.H"
#include "MISCFUNC.H"

#ifdef TK_PORT_POSIXISH
#include <glob.h>
#include <sys/stat.h>
#include <libgen.h>
#endif

#ifdef TK_PORT_MSVC
#include "MSVC/MISCFUNC_MSVC.H"
#endif

void change_name( int num )
{
    int x;
    fadeout( virbuff, pal );
    load_efp( "EFPS/COOL.EFP", picture, 0 );
    FONT_NUM = 0;
    x = 160 - ( str_length( "enter your name" )  / 2 );
    draw_box1( x - 10, 75, 320 - x + 10, 125, 5 );
    memcpy( virbuff, picture, 64000 );
    writefonts2( x, 85, "enter your name", 1 );
    fadein( virbuff, pal );
    k::clear_stack();
    if ( num == 1 )
    readline( x, 100, 10, name1, screen );
    if ( num == 2 )
    readline( x, 100, 10, name2, screen );
    fadeout( virbuff, pal );
}

/**
 * Convert a TK keycode into a string representing it.
 *
 * The caller is responsible for freeing the returned buffer.
 *
 * TODO: Make sure the callers do that; they don't.
 *
 * @param key Keycode.
 * @return String for the keycode.
 */
char *k_2_c( int key )
{
    for ( int a = 0; a < SPEC_K; a++ )
    {
        if ( key == spec[a].num )
        {
            return strdup(spec[a].text);
        }
    }
    char text[15];
    sprintf(text, "%c", key);
    return strdup(text);
}

int get_key( int x, int y )
{
    int a = 0, key = 0, oclock = 0, cnt = 0, pressed = 0;
    while ( !pressed )
    {
        if ( clock() != oclock )
        {
            oclock = clock();
            draw_ball( x, y, cnt );
            cnt ++;
            if ( cnt > 23 ) cnt = 0;
        }
        for ( a = 0; a < 0x80 &&!pressed; a ++  )
        {
            if ( a != k::LEFT_SHIFT )
                if ( k::state[a] )
                {
                    key = a;
                    pressed = 1;
                }
        }
        tk_port::event_tick();
    }
    for ( a = 0; a < 0x80; a ++  )
    k::state[a] = 0;
    return( key );
}

void clear_shit( int y )
{
    int a;
    for ( a = 0; a < 20; a ++  )
    memcpy( screen + ( ( y + a ) *320 )  + 20, picture + ( ( y + a ) *320 )  + 20, 20 );
    for ( a = 0; a < 8; a ++  )
    memcpy( screen + ( ( y + a + 3 ) *320 )  + 190, picture + ( ( y + a + 3 ) *320 )  + 190, 130 );
}

void set_keys()
{
    if ( GAME_MODE != SPLIT_SCREEN )
    {
        aplayer[0]->K_LEFT = keys1.K_LEFT;
        aplayer[0]->K_RIGHT = keys1.K_RIGHT;
        aplayer[0]->K_UP = keys1.K_UP;
        aplayer[0]->K_DOWN = keys1.K_DOWN;
        aplayer[0]->K_SHOOT = keys1.K_SHOOT;
        aplayer[0]->K_SHIFT = keys1.K_SHIFT;
        aplayer[0]->K_STRAFE = keys1.K_STRAFE;
        aplayer[0]->K_LSTRAFE = keys1.K_LSTRAFE;
        aplayer[0]->K_RSTRAFE = keys1.K_RSTRAFE;
    }
    else
    {
        aplayer[0]->K_LEFT = keys2.K_LEFT;
        aplayer[0]->K_RIGHT = keys2.K_RIGHT;
        aplayer[0]->K_UP = keys2.K_UP;
        aplayer[0]->K_DOWN = keys2.K_DOWN;
        aplayer[0]->K_SHOOT = keys2.K_SHOOT;
        aplayer[0]->K_SHIFT = keys2.K_SHIFT;
        aplayer[0]->K_STRAFE = keys2.K_STRAFE;
        aplayer[0]->K_RSTRAFE = keys2.K_RSTRAFE;
        aplayer[0]->K_LSTRAFE = keys2.K_LSTRAFE;
        aplayer[1]->K_LEFT = keys1.K_LEFT;
        aplayer[1]->K_RIGHT = keys1.K_RIGHT;
        aplayer[1]->K_UP = keys1.K_UP;
        aplayer[1]->K_DOWN = keys1.K_DOWN;
        aplayer[1]->K_SHOOT = keys1.K_SHOOT;
        aplayer[1]->K_SHIFT = keys1.K_SHIFT;
        aplayer[1]->K_STRAFE = keys1.K_STRAFE;
        aplayer[1]->K_RSTRAFE = keys1.K_RSTRAFE;
        aplayer[1]->K_LSTRAFE = keys1.K_LSTRAFE;
    }
}

void alusta_client()
{
 int a;

 aplayer[0]->TARGET = 0;
 aplayer[0]->shooted = 0;
 aplayer[0]->hitten = 0;
 aplayer[0]->curr_weapon = 0;
 aplayer[0]->KICK = 0;
 aplayer[0]->HIT = 0;
// aplayer[0]->move( 0, 0, 0 );
 aplayer[0]->move_scr();
 aplayer[0]->load_cnt = 0;
 aplayer[0]->max_energy = 100;
 for ( a = 0; a < DIFF_WEAPONS + 1; a ++  ) aplayer[0]->weapons[a] = 0;
 for ( a = 0; a < DIFF_BULLETS    ; a ++  ) aplayer[0]->bullets[a] = 0;
 aplayer[0]->weapons[0] = 1;
 if (KILLING_MODE==DEATHMATCH)
 {
  aplayer[0]->shield = 30;
  aplayer[0]->energy = ( aplayer[0]->max_energy + ( aplayer[0]->shield*10 )  )  / 3;
 } else {
  aplayer[0]->shield = 0;
 }
}

void alusta_players1()
{
    int a, b;
    strcpy( aplayer[0]->name, name1 );
    if (ACTIVE_PLAYERS==2)
    strcpy( aplayer[1]->name, name2 );
    aplayer[0]->color = 16*3;
    if (ACTIVE_PLAYERS==2)
    aplayer[1]->color = 16*9;
    for ( b = 0; b < ACTIVE_PLAYERS; b ++  )
    {
        aplayer[b]->enabled=1;
        aplayer[b]->cash = 900;
        aplayer[b]->total_kills = 0;
        aplayer[b]->TARGET = 0;
        if ( KILLING_MODE!= DEATHMATCH ) aplayer[b]->shield = 0;
        else aplayer[b]->shield = 30;
        aplayer[b]->DEAD = 0;
        if ( KILLING_MODE!= DEATHMATCH ) aplayer[b]->speed = 2;else aplayer[b]->speed = DEATH_MATCH_SPEED;
        aplayer[b]->max_energy = 100;
        for ( a = 0; a < DIFF_WEAPONS + 1; a ++  ) aplayer[b]->weapons[a] = 0;
        for ( a = 0; a < DIFF_BULLETS; a ++  ) aplayer[b]->bullets[a] = 0;
        aplayer[b]->weapons[0] = 1;
        if ( KILLING_MODE!= DEATHMATCH )
        aplayer[b]->bullets[0] = 10;
    }
    if ( !strcmp( aplayer[0]->name, "iron" )  ) aplayer[0]->cash = 50000;
    if (ACTIVE_PLAYERS==2)
    if ( !strcmp( aplayer[1]->name, "maiden" )  ) aplayer[1]->cash = 50000;
}

void alusta_players2()
{
    int a, b;
    for ( b = 0; b < ACTIVE_PLAYERS; b ++  )
    {
        aplayer[b]->PUSH_POWER = 0;
        aplayer[b]->x = ( pl_start_x[b]*20 );
        aplayer[b]->y = ( pl_start_y[b]*20 );
        aplayer[b]->burning = 0;
        aplayer[b]->shooted = 0;
        aplayer[b]->hitten = 0;
        aplayer[b]->player_kills = 0;
        aplayer[b]->curr_weapon = 0;
        aplayer[b]->KICK = 0;
        aplayer[b]->HIT = 0;
        aplayer[b]->move( 0, 0, 0 );
        aplayer[b]->move_scr();
        aplayer[b]->load_cnt = 0;
        if ( KILLING_MODE!= DEATHMATCH ) aplayer[b]->energy = aplayer[b]->max_energy + ( aplayer[b]->shield*10 );
        else aplayer[b]->energy = ( aplayer[b]->max_energy + ( aplayer[b]->shield*10 )  )  / 3;
        for ( a = 0; a < DIFF_ENEMIES; a ++  )
        aplayer[b]->kills[a] = 0;
    }
}

void two_to_one()
{
    if (aplayer[0]->DEAD || aplayer[1]->DEAD){
    ACTIVE_PLAYERS=1;
    GAME_MODE = ONE_PLAYER;
    if (aplayer[0]->DEAD) {aplayer[0]->enabled=0;aplayer[0]=aplayer[1];}
                   else aplayer[1]->enabled=0;
    }
}

void define_bullet_types()
{
    strcpy( bullet_type[0].name, "fist" );
    bullet_type[0].type = NORMAL;
    bullet_type[0].stop = 1;
    bullet_type[0].speed = 5;
    bullet_type[0].power = 3;
    bullet_type[0].push_power = 3;
    bullet_type[0].time = 1;
    bullet_type[0].sprite = 0;
    bullet_type[0].light = 0;
    bullet_type[0].effect = 0;
    bullet_type[0].mul = 1;
    bullet_type[0].sleep_time = 0;
    strcpy( bullet_type[1].name, "9mm bullets" );
    bullet_type[1].type = NORMAL;
    bullet_type[1].stop = 1;
    bullet_type[1].speed = 6*18;
    bullet_type[1].power = 5;
    bullet_type[1].push_power = 2;
    bullet_type[1].time = 1;
    bullet_type[1].sprite = 0;
    bullet_type[1].light = 0;
    bullet_type[1].effect = 0;
    bullet_type[1].cost = 3;
    bullet_type[1].amount_in_crate = 50;
    bullet_type[1].mul = 1;
    bullet_type[1].sleep_time = 0;
    bullet_type[1].max = 300;
    strcpy( bullet_type[2].name, "12mm bullets" );
    bullet_type[2].type = NORMAL;
    bullet_type[2].stop = 1;
    bullet_type[2].speed = 7*18;
    bullet_type[2].power = 9;
    bullet_type[2].push_power = 2;
    bullet_type[2].time = 1;
    bullet_type[2].sprite = 0;
    bullet_type[2].light = 0;
    bullet_type[2].effect = 0;
    bullet_type[2].cost = 5;
    bullet_type[2].amount_in_crate = 50;
    bullet_type[2].mul = 1;
    bullet_type[2].sleep_time = 0;
    bullet_type[2].max = 300;
    strcpy( bullet_type[3].name, "shotgun shells" );
    bullet_type[3].type = NORMAL;
    bullet_type[3].stop = 1;
    bullet_type[3].speed = 6*18;
    bullet_type[3].power = 3;
    bullet_type[3].push_power = 3;
    bullet_type[3].time = 1;
    bullet_type[3].sprite = 0;
    bullet_type[3].light = 0;
    bullet_type[3].effect = 0;
    bullet_type[3].cost = 6;
    bullet_type[3].amount_in_crate = 20;
    bullet_type[3].mul = 1;
    bullet_type[3].sleep_time = 0;
    bullet_type[3].max = 300;
    strcpy( bullet_type[4].name, "light grenades" );
    bullet_type[4].type = EXPLOSIVE;
    bullet_type[4].stop = 1;
    bullet_type[4].speed = 8;
    bullet_type[4].power = 0;
    bullet_type[4].push_power = 0;
    bullet_type[4].time = ( 15*18 )  / 8;
    bullet_type[4].explos = 1;
    bullet_type[4].sprite = grenadespr;
    bullet_type[4].xs = 7;
    bullet_type[4].ys = 7;
    bullet_type[4].xsize = 7;
    bullet_type[4].offs = 0;
    bullet_type[4].light = &normal_l;
    bullet_type[4].lsize = 3;
    bullet_type[4].llum = 0;
    bullet_type[4].ladd = 0;
    bullet_type[4].effect = SMOKE2;
    bullet_type[4].cost = 70;
    bullet_type[4].amount_in_crate = 15;
    bullet_type[4].mul = 1;
    bullet_type[4].sleep_time = 0;
    bullet_type[4].max = 150;
    strcpy( bullet_type[5].name, "medium grenades" );
    bullet_type[5].type = EXPLOSIVE;
    bullet_type[5].stop = 1;
    bullet_type[5].speed = 8;
    bullet_type[5].power = 0;
    bullet_type[5].push_power = 0;
    bullet_type[5].time = ( 15*18 )  / 8;
    bullet_type[5].explos = 2;
    bullet_type[5].sprite = grenadespr;
    bullet_type[5].xs = 7;
    bullet_type[5].ys = 7;
    bullet_type[5].xsize = 7;
    bullet_type[5].offs = 0;
    bullet_type[5].light = &normal_l;
    bullet_type[5].lsize = 3;
    bullet_type[5].llum = 0;
    bullet_type[5].ladd = 0;
    bullet_type[5].effect = SMOKE2;
    bullet_type[5].cost = 100;
    bullet_type[5].amount_in_crate = 10;
    bullet_type[5].mul = 1;
    bullet_type[5].sleep_time = 0;
    bullet_type[5].max = 125;
    strcpy( bullet_type[6].name, "heavy grenades" );
    bullet_type[6].type = EXPLOSIVE;
    bullet_type[6].stop = 1;
    bullet_type[6].speed = 8;
    bullet_type[6].power = 0;
    bullet_type[6].push_power = 0;
    bullet_type[6].time = ( 15*18 )  / 8;
    bullet_type[6].explos = 4;
    bullet_type[6].sprite = grenadespr;
    bullet_type[6].xs = 7;
    bullet_type[6].ys = 7;
    bullet_type[6].xsize = 7;
    bullet_type[6].offs = 0;
    bullet_type[6].light = &normal_l;
    bullet_type[6].lsize = 4;
    bullet_type[6].llum = 0;
    bullet_type[6].ladd = 0;
    bullet_type[6].effect = SMOKE2;
    bullet_type[6].cost = 200;
    bullet_type[6].amount_in_crate = 5;
    bullet_type[6].mul = 1;
    bullet_type[6].sleep_time = 0;
    bullet_type[6].max = 100;
    strcpy( bullet_type[7].name, "C4-explosives" );
    bullet_type[7].type = EXPLOSIVE;
    bullet_type[7].stop = 0;
    bullet_type[7].speed = 0;
    bullet_type[7].power = 0;
    bullet_type[7].push_power = 0;
    bullet_type[7].time = 100;
    bullet_type[7].explos = 4;
    bullet_type[7].sprite = c4spr;
    bullet_type[7].xs = 8;
    bullet_type[7].ys = 8;
    bullet_type[7].xsize = 8;
    bullet_type[7].offs = 0;
    bullet_type[7].light = 0;
    bullet_type[7].lsize = 0;
    bullet_type[7].effect = 0;
    bullet_type[7].cost = 150;
    bullet_type[7].amount_in_crate = 5;
    bullet_type[7].mul = 1;
    bullet_type[7].sleep_time = 0;
    bullet_type[7].max = 100;
    strcpy( bullet_type[8].name, "Gas" );
    bullet_type[8].type = INCENDIARY;
    bullet_type[8].stop = 1;
    bullet_type[8].speed = 5;
    bullet_type[8].power = 0.4f;
    bullet_type[8].push_power = 0;
    bullet_type[8].time = ( 5*18 )  / 5;
    bullet_type[8].explos = 0;
    bullet_type[8].sprite = 0;
    bullet_type[8].light = &explo_l;
    bullet_type[8].lsize = 2;
    bullet_type[8].llum =  - 10;
    bullet_type[8].ladd = 0.7f;
    bullet_type[8].effect = 0;
    bullet_type[8].cost = 6;
    bullet_type[8].amount_in_crate = 500;// div mul = 50
    bullet_type[8].mul = 10;
    bullet_type[8].sleep_time = 0;
    bullet_type[8].max = 300;
    strcpy( bullet_type[9].name, "Mines" );
    bullet_type[9].type = EXPLOSIVE;
    bullet_type[9].stop = 1;
    bullet_type[9].speed = 0;
    bullet_type[9].power = 0;
    bullet_type[9].push_power = 0;
    bullet_type[9].time = 2000;
    bullet_type[9].explos = 1;
    bullet_type[9].sprite = minespr;
    bullet_type[9].xs = 7;
    bullet_type[9].ys = 7;
    bullet_type[9].xsize = 7;
    bullet_type[9].offs = 0;
    bullet_type[9].light = 0;
    bullet_type[9].lsize = 0;
    bullet_type[9].effect = 0;
    bullet_type[9].cost = 150;
    bullet_type[9].amount_in_crate = 5;
    bullet_type[9].mul = 1;
    bullet_type[9].sleep_time = 25;
    bullet_type[9].max = 100;
    strcpy( bullet_type[10].name, "Explosive ray" );
    bullet_type[10].type = NORMAL;
    bullet_type[10].stop = 1;
    bullet_type[10].speed = 20;
    bullet_type[10].power = 30;
    bullet_type[10].push_power = 8;
    bullet_type[10].time = 0; // define in new_bullet()
    bullet_type[10].sprite = 0;
    bullet_type[10].effect = EXPLO;
    bullet_type[10].mul = 1;
    bullet_type[10].sleep_time = 0;
}

void define_effects()
{
    effect_type[0].index = 0;
    effect_type[0].tcount = 4;
    effect_type[0].order = 1;
    effect_type[0].delay = 2;
    effect_type[0].random = 0;
    effect_type[0].xs = 12;
    effect_type[0].ys = 12;
    effect_type[0].offs = 0;
    effect_type[1].index = 1;
    effect_type[1].tcount = 4;
    effect_type[1].order = 0;
    effect_type[1].delay = 2;
    effect_type[1].random = 0;
    effect_type[1].xs = 12;
    effect_type[1].ys = 12;
    effect_type[1].offs = 13*4;
    effect_type[2].index = 2;
    effect_type[2].tcount = 9;
    effect_type[2].order = 1;
    effect_type[2].delay = 2;
    effect_type[2].random = 0;
    effect_type[2].xs = 25;
    effect_type[2].ys = 25;
    effect_type[2].offs = 320*13;
    effect_type[3].index = 3;
    effect_type[3].tcount = 8;
    effect_type[3].order = 1;
    effect_type[3].delay = 2;
    effect_type[3].random = 0;
    effect_type[3].xs = 12;
    effect_type[3].ys = 12;
    effect_type[3].offs = 13*8;
    effect_type[4].index = 4;
    effect_type[4].tcount = 12;
    effect_type[4].order = 1;
    effect_type[4].delay = 5;
    effect_type[4].random = 1;
    effect_type[4].xs = 20;
    effect_type[4].ys = 20;
    effect_type[4].offs = 320*39;
    effect_type[5].index = 5;
    effect_type[5].tcount = 8;
    effect_type[5].order = 0;
    effect_type[5].delay = 20;
    effect_type[5].random = 1;
    effect_type[5].xs = 6;
    effect_type[5].ys = 6;
    effect_type[5].offs = 13*20;
    effect_type[6].index = 6;
    effect_type[6].tcount = 4;
    effect_type[6].order = 1;
    effect_type[6].delay = 2;
    effect_type[6].random = 0;
    effect_type[6].xs = 12;
    effect_type[6].ys = 12;
    effect_type[6].offs = 13*16;
    effect_type[7].index = 7;
    effect_type[7].tcount = 12;
    effect_type[7].order = 1;
    effect_type[7].delay = 2;
    effect_type[7].random = 0;
    effect_type[7].xs = 20;
    effect_type[7].ys = 20;
    effect_type[7].offs = 320*39;
    effect_type[8].index = 8;
    effect_type[8].tcount = 7;
    effect_type[8].order = 1;
    effect_type[8].delay = 2;
    effect_type[8].random = 0;
    effect_type[8].xs = 20;
    effect_type[8].ys = 20;
    effect_type[8].offs = 320*60;
    effect_type[9].index = 9;
    effect_type[9].tcount = 4;
    effect_type[9].order = 0;
    effect_type[9].delay = 50;
    effect_type[9].random = 1;
    effect_type[9].xs = 6;
    effect_type[9].ys = 6;
    effect_type[9].offs = ( 13*20 )  + ( 320*7 );
}

void explo( int x, int y, int size, int index )
{
    int a, rays;
    rays = size*5;
    for ( a = 0; a < rays; a ++  )
    new_bullet( EXPLO_RAY, x, y, a*( float ) ( 360 / rays ), size,index);
    for ( a = 0; a < ( rand()  % 5 )  + 1; a ++  )
    if ( rand()  % 2 )
    new_effect( &yellow_l, METAL, x, y, rand()  % 360, ( rand()  % 3 )  + 4 );
    else
    new_effect( &red_l, METAL, x, y, rand()  % 360, ( rand()  % 3 )  + 4 );

   if (GAME_MODE!=SPLIT_SCREEN)
    MIDASplaySample( samplep[EXPLOWAV], MIDAS_CHANNEL_AUTO, 0, 14000 - ( size*1000 ), get_volume(aplayer[0]->x,aplayer[0]->y,x,y), get_pan(aplayer[0]->x,x));
   else
    MIDASplaySample( samplep[EXPLOWAV], MIDAS_CHANNEL_AUTO, 0, 14000 - ( size*1000 ), EFFECT_VOLUME, 0 );
}

/**
 * Populate `filelist` (list allocated by the caller) with filenames (note: _not_ paths)
 * matching the given `wildcard` pattern and `type`.
 *
 * @param no Populated with the number of files found.
 * @param filelist Populated with the file info.
 * @param wildcard Wildcard pattern
 * @param type _A_SUBDIR or _A_NORMAL.
 */
void list_files( int *no, vile *filelist, const char *wildcard, int type )
{
#if defined(TK_PORT_POSIXISH)
    glob_t globp;
    glob(wildcard, 0, NULL, &globp);
    for (int i = 0; i < globp.gl_pathc; i++) {
        struct stat file_stat;
        char *file_path = globp.gl_pathv[i];
        if (stat(file_path, &file_stat) == 0) {
            if (
                    (type == _A_SUBDIR && S_ISDIR(file_stat.st_mode)) ||
                    (type == _A_NORMAL && S_ISREG(file_stat.st_mode))
                    ) {
                strncpy(filelist[*no].name, basename(file_path), 12);
                (*no)++;
            }
        }
    }
    globfree(&globp);
#elif defined(TK_PORT_MSVC)
    return list_files_msvc( no, filelist, wildcard, type );
#endif
}

#ifndef TK_PORT_MSVC
// windows.h has these already
char *strlwr( char *str )
{
    char *p = str;

    while ( *p )
    {
        *p = (char) tolower( (unsigned char) *p );
        p++;
    }

    return str;
}

char *strupr( char *str )
{
    char *p = str;

    while ( *p )
    {
        *p = (char) toupper( (unsigned char) *p );
        p++;
    }

    return str;
}
#endif
