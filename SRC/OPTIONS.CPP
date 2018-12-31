#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "EFP/EFP.H"
#include "DEFINES.H"
#include "TYPES.H"
#include "GLOBVAR.H"
#include "MISCFUNC.H"
#include "FADE.H"
#include "DRAW.H"
#include "WRITE.H"

void setup_ipx();
void desetup_ipx();
int select_server();

extern void game();

void save_options()
{
    FILE *cfg;
    cfg = fopen( "OPTIONS.CFG", "wb" );
    fwrite( &keys1, sizeof( class Keys ), 1, cfg );
    fwrite( &keys2, sizeof( class Keys ), 1, cfg );
    fwrite( name1, sizeof( name1 ), 1, cfg );
    fwrite( name2, sizeof( name2 ), 1, cfg );
    fwrite( &DARK_MODE, sizeof( DARK_MODE ), 1, cfg );
    fwrite( &LIGHT_EFFECTS, sizeof( LIGHT_EFFECTS ), 1, cfg );
    fwrite( &SHADOWS, sizeof( SHADOWS ), 1, cfg );
    fwrite( &MUSIC_VOLUME, sizeof( MUSIC_VOLUME ), 1, cfg );
    fwrite( &EFFECT_VOLUME, sizeof( EFFECT_VOLUME ), 1, cfg );
    fwrite( &ENEMIES_ON_GAME, sizeof( ENEMIES_ON_GAME ), 1, cfg );
    fwrite( DEATH_MATCH_LEVEL, sizeof( DEATH_MATCH_LEVEL ), 1, cfg );
    fwrite( &DEATH_MATCH_EPISODE, sizeof( DEATH_MATCH_EPISODE ), 1, cfg );
    fwrite( &DEATH_MATCH_SPEED, sizeof( DEATH_MATCH_SPEED ), 1, cfg );
    fwrite( &saved_killing_mode, sizeof( saved_killing_mode ), 1, cfg );
    fwrite( &saved_game_mode, sizeof( saved_game_mode ), 1, cfg );
    fclose( cfg );
}

void load_options()
{
    FILE *cfg;
    cfg = fopen( "OPTIONS.CFG", "rb" );
    if ( cfg )
    {
        fread( &keys1, sizeof( class Keys ), 1, cfg );
        fread( &keys2, sizeof( class Keys ), 1, cfg );
        fread( name1, sizeof( name1 ), 1, cfg );
        fread( name2, sizeof( name2 ), 1, cfg );
        fread( &DARK_MODE, sizeof( DARK_MODE ), 1, cfg );
        fread( &LIGHT_EFFECTS, sizeof( LIGHT_EFFECTS ), 1, cfg );
        fread( &SHADOWS, sizeof( SHADOWS ), 1, cfg );
        fread( &MUSIC_VOLUME, sizeof( MUSIC_VOLUME ), 1, cfg );
        fread( &EFFECT_VOLUME, sizeof( EFFECT_VOLUME ), 1, cfg );
        fread( &ENEMIES_ON_GAME, sizeof( ENEMIES_ON_GAME ), 1, cfg );
        fread( DEATH_MATCH_LEVEL, sizeof( DEATH_MATCH_LEVEL ), 1, cfg );
        fread( &DEATH_MATCH_EPISODE, sizeof( DEATH_MATCH_EPISODE ), 1, cfg );
        fread( &DEATH_MATCH_SPEED, sizeof( DEATH_MATCH_SPEED ), 1, cfg );
        fread( &saved_killing_mode, sizeof( saved_killing_mode ), 1, cfg );
        fread( &saved_game_mode, sizeof( saved_game_mode ), 1, cfg );
        fclose( cfg );
    }
}

int choose_load_file( char *name, int episode )
{
    int a, b, sel = 0, pos = 0, cnt = 0, oclock = 0;
    int files = files_in_episode[episode];
    load_efp( "EFPS/COOL.EFP", picture, 0 );
    draw_box1( 60, 10, 320 - 60, 190, 15 );
    first = 1;
    while ( !k::state(k::ESC)&&!k::state(k::ENTER) )
    {
        memcpy( virbuff, picture, 64000 );
        FONT_NUM = 0;
        writefonts2( 160 - ( str_length( "select level:" )  >> 1 ), 20, "select level:", 1 );
        b = files;
        if ( b > 15 ) b = 15;
        FONT_NUM = 1;
        for ( a = 0; a < b; a ++  )
        if ( a + pos == sel )
            writefonts( 160 - ( str_length( filelist[episode][a + pos].name )  >> 1 ), 50 + a*8, filelist[episode][a + pos].name, M_YELLOW );
        else
            writefonts( 160 - ( str_length( filelist[episode][a + pos].name )  >> 1 ), 50 + a*8, filelist[episode][a + pos].name, M_BLUE );
        if ( pos > 0 ) writefonts( 156, 42, "", M_BLUE );
        if ( files > 15 ) if ( pos < files - 15 ) writefonts( 156, 50 + 15*8, "", M_BLUE );
        if ( first == 1 )
        {
            fadein( virbuff, pal ); first = 0;
        }
        memcpy( screen, virbuff, 64000 );
        while( !k::state(k::ARROW_UP)&&!k::state(k::ARROW_DOWN)&&!k::state(k::ENTER) )
        {
            if ( clock() != oclock )
            {
                oclock = clock();
                draw_ball( 65, 46 + ((sel - pos) * 8), cnt );
                draw_ball( 320 - 85, 46 + ((sel - pos) * 8), cnt );
                cnt++;
                if ( cnt > 23 ) cnt = 0;
            }
            tk_port::event_tick();
        }
        if ( k::state(k::ARROW_UP) )
        {
            if ( sel > 0 ) sel --;
            k::clear(k::ARROW_UP);
        }
        if ( k::state(k::ARROW_DOWN) )
        {
            if ( sel < files - 1 ) sel ++;
            k::clear(k::ARROW_DOWN);
        }
        pos = sel - 8;
        if ( pos < 0 ) pos = 0;
        if ( files > 15 ) if ( pos > files - 15 ) pos = files - 15;
        if ( files <= 15 ) if ( pos > 0 ) pos = 0;
    }
    a = 0;
    strcpy( name, filelist[episode][sel].name ); a = 1; k::clear(k::ENTER);
    return( a );
}

int choose_episode()
{
    int a, b, sel = 0, pos = 0, cnt = 0, oclock = 0;
    char text[20];
    load_efp( "EFPS/COOL.EFP", picture, 0 );
    draw_box1( 60, 10, 320 - 60, 190, 15 );
    first = 1;
    k::clear(k::ESC);
    k::clear(k::ENTER);
    fadeout( virbuff, pal );
    while ( !k::state(k::ESC)&&!k::state(k::ENTER) )
    {
        memcpy( virbuff, picture, 64000 );
        FONT_NUM = 0;
        writefonts2( 160 - ( str_length( "select episode" )  >> 1 ), 20, "select episode", 1 );
        b = dirs;
        if ( b > 7 ) b = 7;
        for ( a = 0; a < b; a ++  )
        {
            if ( a + pos == sel )
            {
                FONT_NUM = 0;
                writefonts2( 160 - ( str_length( dirlist[a + pos].name )  >> 1 ), 50 + a*16, dirlist[a + pos].name, 1 );
            }
            else
            {
                FONT_NUM = 2;
                writefonts2( 160 - ( str_length( dirlist[a + pos].name )  >> 1 ), 50 + a*16, dirlist[a + pos].name, 1 );
            }
        }
        FONT_NUM = 1;
        sprintf( text, "%d", files_in_episode[sel + 1] );
        strcat( text, " levels" );
        writefonts( 70, 175, text, M_BLUE );
        if ( first == 1 )
        {
            fadein( virbuff, pal ); first = 0;
        }
        memcpy( screen, virbuff, 64000 );
        while( !k::state(k::ARROW_UP)&&!k::state(k::ARROW_DOWN)&&!k::state(k::ENTER)&&!k::state(k::ESC) )
        {
            if ( clock() != oclock )
            {
                oclock = clock();
                draw_ball( 65, 48 + ((sel - pos) * 16), cnt );
                draw_ball( 320 - 85, 48 + ((sel - pos) * 16), cnt );
                cnt++;
                if ( cnt > 23 ) cnt = 0;
            }
            tk_port::event_tick();
        }
        if ( k::state(k::ARROW_UP) )
        {
            if ( sel > 0 ) sel --;
            k::clear(k::ARROW_UP);
        }
        if ( k::state(k::ARROW_DOWN) )
        {
            if ( sel < dirs - 1 ) sel ++;
            k::clear(k::ARROW_DOWN);
        }
        if ( k::state(k::ESC) )
        {
            fadeout( virbuff, pal );
            return INVALID_EPISODE;
        }
        pos = sel - 3;
        if ( pos < 0 ) pos = 0;
        if ( dirs > 7 ) if ( pos > dirs - 7 ) pos = dirs - 7;
        if ( dirs <= 7 ) if ( pos > 0 ) pos = 0;
    }
    k::clear(k::ENTER);
    return( sel );
}

void options_scr( int opt, const char *filename )
{
    load_efp( filename, picture, 0 );
    load_efp_pal( "EFPS/WARE.EFP", pal );
    draw_box1( 45, 90 - ( ( opt*15 )  / 2 ), 320 - 45, 110 + ( ( opt*15 )  / 2 ), 13 );
    first = 1;
}

void multip_options_scr( int opt, const char *filename )
{
    load_efp( filename, picture, 0 );
    load_efp_pal( "EFPS/WARE.EFP", pal );
    draw_box1( 10, 90 - ( ( opt*15 )  / 2 ), 320 - 10, 110 + ( ( opt*15 )  / 2 ), 13 );
    first = 1;
}

void player_options()
{
    int yoffs = ( 15*num_player_options )  / 2, a, cnt = 0, selected = 0, quit = 0, oclock = 0;
    fadeout( virbuff, pal );
    options_scr( num_player_options, "EFPS/COOL.EFP" );
    k::clear(k::ENTER);
    while( !quit )
    {
        memcpy( virbuff, picture, 64000 );
        for ( a = 0; a < num_player_options; a ++  )
        {
            if ( a == selected )
            {
                FONT_NUM = 0;
                writefonts2( 160 - ( str_length( player_opt_text[a] )  / 2 ), ( 100 - yoffs )  + ( a*15 ), player_opt_text[a], 1 );
            }
            else
            {
                FONT_NUM = 2;
                writefonts2( 160 - ( str_length( player_opt_text[a] )  / 2 ), ( 100 - yoffs )  + ( a*15 ), player_opt_text[a], M_RED );
            }
        }
        if ( first == 1 )
        {
            fadein( virbuff, pal ); first = 0;
        }
        memcpy( screen, virbuff, 64000 );
        while ( !k::state(k::ESC)&&!k::state(k::ARROW_UP)&&!k::state(k::ARROW_DOWN)&&!k::state(k::ENTER) )
        {
            if ( clock() != oclock )
            {
                oclock = clock();
                draw_ball( 65, ( 98 - yoffs )  + ( selected*15 ), cnt );
                draw_ball( 320 - 85, ( 98 - yoffs )  + ( selected*15 ), cnt );
                cnt ++;
                if ( cnt > 23 ) cnt = 0;
            }
            tk_port::event_tick();
        };
        MIDASplaySample( samplep[KLIKWAV], MIDAS_CHANNEL_AUTO, 0, 22500, EFFECT_VOLUME, MIDAS_PAN_MIDDLE );
        if ( k::state(k::ARROW_UP) )
        {
            selected --;if ( selected < 0 ) selected = num_player_options - 1; k::clear(k::ARROW_UP);
        }
        if ( k::state(k::ARROW_DOWN) )
        {
            selected ++;if ( selected >= num_player_options ) selected = 0; k::clear(k::ARROW_DOWN);
        }
        if ( k::state(k::ENTER) )
        {
            if ( selected == 0 )
            {
                keys1.change(); options_scr( num_player_options, "EFPS/COOL.EFP" );
            }
            if ( selected == 1 )
            {
                keys2.change(); options_scr( num_player_options, "EFPS/COOL.EFP" );
            }
            if ( selected == 2 )
            {
                change_name( 1 ); options_scr( num_player_options, "EFPS/COOL.EFP" );
            }
            if ( selected == 3 )
            {
                change_name( 2 ); options_scr( num_player_options, "EFPS/COOL.EFP" );
            }
            if ( selected == 4 )
            {
                quit = 1;
            }
        }
        if ( k::state(k::ESC) )
        {
            quit = 1;k::clear(k::ESC);
        }
    }
    fadeout( virbuff, pal );
}

void game_options()
{
    int yoffs = ( 15*num_game_options )  / 2, a, cnt = 0, selected = 0, quit = 0, oclock = 0;
    fadeout( virbuff, pal );
    options_scr( num_game_options, "EFPS/COOL.EFP" );
    k::clear(k::ENTER);
    while( !quit )
    {
        memcpy( virbuff, picture, 64000 );
        if ( DARK_MODE ) strcpy(game_opt_text[0],"darkness on");
        if ( !DARK_MODE ) strcpy(game_opt_text[0],"darkness off" );
        if ( LIGHT_EFFECTS ) strcpy(game_opt_text[1],"lights on" );
        if ( !LIGHT_EFFECTS ) strcpy(game_opt_text[1],"lights off" );
        if ( SHADOWS ) strcpy(game_opt_text[2],"shadows on" );
        if ( !SHADOWS ) strcpy(game_opt_text[2],"shadows off" );
        for ( a = 0; a < num_game_options; a ++  )
        {
            if ( a == selected )
            {
                FONT_NUM = 0;
                writefonts2( 160 - ( str_length( game_opt_text[a] )  / 2 ), ( 100 - yoffs )  + ( a*15 ), game_opt_text[a], 1 );
            }
            else
            {
                FONT_NUM = 2;
                writefonts2( 160 - ( str_length( game_opt_text[a] )  / 2 ), ( 100 - yoffs )  + ( a*15 ), game_opt_text[a], M_RED );
            }
        }
        if ( first == 1 )
        {
            fadein( virbuff, pal ); first = 0;
        }
        memcpy( screen, virbuff, 64000 );
        while ( !k::state(k::ESC)&&!k::state(k::ARROW_UP)&&!k::state(k::ARROW_DOWN)&&!k::state(k::ENTER) )
        {
            if ( clock() != oclock )
            {
                oclock = clock();
                draw_ball( 65, ( 98 - yoffs )  + ( selected*15 ), cnt );
                draw_ball( 320 - 85, ( 98 - yoffs )  + ( selected*15 ), cnt );
                cnt ++;
                if ( cnt > 23 ) cnt = 0;
            }
            tk_port::event_tick();
        };
        MIDASplaySample( samplep[KLIKWAV], MIDAS_CHANNEL_AUTO, 0, 22500, EFFECT_VOLUME, MIDAS_PAN_MIDDLE );
        if ( k::state(k::ARROW_UP) )
        {
            selected --;if ( selected < 0 ) selected = num_game_options - 1; k::clear(k::ARROW_UP);
        }
        if ( k::state(k::ARROW_DOWN) )
        {
            selected ++;if ( selected >= num_game_options ) selected = 0; k::clear(k::ARROW_DOWN);
        }
        if ( k::state(k::ENTER) )
        {
            if ( selected == 0 )
            {
                if ( !DARK_MODE ) DARK_MODE = 1;else DARK_MODE = 0;k::clear(k::ENTER);
            }
            if ( selected == 1 )
            {
                if ( !LIGHT_EFFECTS ) LIGHT_EFFECTS = 1;else LIGHT_EFFECTS = 0;k::clear(k::ENTER);
            }
            if ( selected == 2 )
            {
                if ( !SHADOWS ) SHADOWS = 1;else SHADOWS = 0;k::clear(k::ENTER);
            }
            if ( selected == 3 )
            {
                quit = 1;
            }
        }
        if ( k::state(k::ESC) )
        {
            quit = 1;k::clear(k::ESC);
        }
    }
    fadeout( virbuff, pal );
}

void sound_options()
{
    int yoffs = ( 15*num_sound_options )  / 2, a, cnt = 0, selected = 0, quit = 0, oclock = 0;
    char text[30], text2[30];
    fadeout( virbuff, pal );
    options_scr( num_sound_options, "EFPS/SUBBARI.EFP" );
    k::clear(k::ENTER);
    while( !quit )
    {
        memcpy( virbuff, picture, 64000 );
        strcpy( text, "music volume " );
        sprintf( text2, "%.0f", ( float ) MUSIC_VOLUME / 63*100 );
        strcat( text2, "%" );
        strcat( text, text2 );
        strcpy(sound_opt_text[0],text );
        strcpy( text, "effect volume " );
        sprintf( text2, "%.0f", ( float ) EFFECT_VOLUME / 63*100 );
        strcat( text2, "%" );
        strcat( text, text2 );
        strcpy(sound_opt_text[1],text );
        for ( a = 0; a < num_sound_options; a ++  )
        {
            if ( a == selected )
            {
                FONT_NUM = 0;
                writefonts2( 160 - ( str_length( sound_opt_text[a] )  / 2 ), ( 100 - yoffs )  + ( a*15 ), sound_opt_text[a], 1 );
            }
            else
            {
                FONT_NUM = 2;
                writefonts2( 160 - ( str_length( sound_opt_text[a] )  / 2 ), ( 100 - yoffs )  + ( a*15 ), sound_opt_text[a], M_RED );
            }
        }
        if ( first == 1 )
        {
            fadein( virbuff, pal ); first = 0;
        }
        memcpy( screen, virbuff, 64000 );
        tk_port::event_tick();  // reset keystates before loop
        while ( !k::state(k::ESC)&&!k::state(k::ARROW_UP)&&!k::state(k::ARROW_DOWN)&&!k::state(k::ARROW_LEFT)&&!k::state(k::ARROW_RIGHT)&&!k::state(k::ENTER) )
        {
            if ( clock() != oclock )
            {
                oclock = clock();
                draw_ball( 55, ( 98 - yoffs )  + ( selected*15 ), cnt );
                draw_ball( 320 - 75, ( 98 - yoffs )  + ( selected*15 ), cnt );
                cnt ++;
                if ( cnt > 23 ) cnt = 0;
            }
            tk_port::event_tick();
        }
        MIDASplaySample( samplep[KLIKWAV], MIDAS_CHANNEL_AUTO, 0, 22500, EFFECT_VOLUME, MIDAS_PAN_MIDDLE );
        if ( k::state(k::ARROW_UP) )
        {
            selected --;if ( selected < 0 ) selected = num_sound_options - 1; k::clear(k::ARROW_UP);
        }
        if ( k::state(k::ARROW_DOWN) )
        {
            selected ++;if ( selected >= num_sound_options ) selected = 0; k::clear(k::ARROW_DOWN);
        }
        if ( k::state(k::ARROW_LEFT) )
        {
            if ( selected == 0 ) MUSIC_VOLUME -= 5;
            if ( selected == 1 ) EFFECT_VOLUME -= 5;
            if ( MUSIC_VOLUME < 0 ) MUSIC_VOLUME = 0;
            if ( EFFECT_VOLUME < 0 ) EFFECT_VOLUME = 0;
            if ( selected == 0 ) MIDASsetMusicVolume( ph, MUSIC_VOLUME );
            k::clear(k::ARROW_LEFT);
        }
        if ( k::state(k::ARROW_RIGHT) )
        {
            if ( selected == 0 ) MUSIC_VOLUME += 5;
            if ( selected == 1 ) EFFECT_VOLUME += 5;
            if ( MUSIC_VOLUME > 63 ) MUSIC_VOLUME = 63;
            if ( EFFECT_VOLUME > 63 ) EFFECT_VOLUME = 63;
            if ( selected == 0 ) MIDASsetMusicVolume( ph, MUSIC_VOLUME );
            k::clear(k::ARROW_RIGHT);
        }
        if ( k::state(k::ENTER) ) if ( selected == 2 )
        {
            quit = 1;
        }
        if ( k::state(k::ESC) )
        {
            quit = 1;k::clear(k::ESC);
        }
    }
    fadeout( virbuff, pal );
}

int deathmatch_options()
{
    int start=0;
    int yoffs = ( 15*num_deathmatch_options )  / 2, a, cnt = 0, selected = 0, quit = 0, oclock = 0;
    fadeout( virbuff, pal );
    options_scr( num_deathmatch_options, "EFPS/COOL.EFP" );
    k::clear(k::ENTER);
    while( !quit )
    {
        memcpy( virbuff, picture, 64000 );
        if ( DEATH_MATCH_SPEED == 2 )
             strcpy(deathmatch_opt_text[1],"normal moving");
        else strcpy(deathmatch_opt_text[1],"fast moving");
        if ( ENEMIES_ON_GAME )
             strcpy(deathmatch_opt_text[2],"enemies on" );
        else strcpy(deathmatch_opt_text[2],"enemies off" );
        for ( a = 0; a < num_deathmatch_options; a ++  )
        {
            if ( a == selected )
            {
                FONT_NUM = 0;
                writefonts2( 160 - ( str_length( deathmatch_opt_text[a] )  / 2 ), ( 100 - yoffs )  + ( a*15 ), deathmatch_opt_text[a], 1 );
            }
            else
            {
                FONT_NUM = 2;
                writefonts2( 160 - ( str_length( deathmatch_opt_text[a] )  / 2 ), ( 100 - yoffs )  + ( a*15 ), deathmatch_opt_text[a], 1 );
            }
        }
        if ( first == 1 )
        {
            fadein( virbuff, pal ); first = 0;
        }
        memcpy( screen, virbuff, 64000 );
        while ( !k::state(k::ESC)&&!k::state(k::ARROW_UP)&&!k::state(k::ARROW_DOWN)&&!k::state(k::ENTER) )
        {
            if ( clock() != oclock )
            {
                oclock = clock();
                draw_ball( 50, ( 98 - yoffs )  + ( selected*15 ), cnt );
                draw_ball( 320 - 70, ( 98 - yoffs )  + ( selected*15 ), cnt );
                cnt ++;
                if ( cnt > 23 ) cnt = 0;
            }
            tk_port::event_tick();
        };
        MIDASplaySample( samplep[KLIKWAV], MIDAS_CHANNEL_AUTO, 0, 22500, EFFECT_VOLUME, MIDAS_PAN_MIDDLE );
        if ( k::state(k::ARROW_UP) )
        {
            selected --;if ( selected < 0 ) selected = num_deathmatch_options - 1; k::clear(k::ARROW_UP);
        }
        if ( k::state(k::ARROW_DOWN) )
        {
            selected ++;if ( selected >= num_deathmatch_options ) selected = 0; k::clear(k::ARROW_DOWN);
        }
        if ( k::state(k::ENTER) )
        {
            k::clear(k::ENTER);
            if ( selected == 0 ) { quit = 1;start = 1;}
            if ( selected == 1 ) DEATH_MATCH_SPEED = DEATH_MATCH_SPEED == 2 ? 3 : 2;
            if ( selected == 2 ) ENEMIES_ON_GAME = ENEMIES_ON_GAME ? 0 : 1;
            if ( selected == 3 ) quit = 1;

        }
        if ( k::state(k::ESC) )
        {
            quit = 1;k::clear(k::ESC);
        }
    }
    if (! start) fadeout( virbuff, pal );
    save_options();
    return(start);
}

void multiplayer_options()
{
    int yoffs = ( 15*num_multip_options )  / 2, a, cnt = 0, selected = 0, quit = 0, oclock = 0;
    int dontfade = 0;
    char text[50];
    fadeout( virbuff, pal );
    multip_options_scr( num_multip_options, "EFPS/COOL.EFP" );
    k::clear(k::ENTER);
    GAME_MODE = saved_game_mode;
    KILLING_MODE = saved_killing_mode;
    while( !quit )
    {
        memcpy( virbuff, picture, 64000 );
        if ( KILLING_MODE == COOPERATIVE &&DEATH_MATCH_EPISODE == 0 )
        {
            DEATH_MATCH_EPISODE = 1;
            strcpy( DEATH_MATCH_LEVEL, filelist[DEATH_MATCH_EPISODE][0].name );
        }
        strcpy( text, "episode: " );
        if ( DEATH_MATCH_EPISODE > 0 ) strcat( text, dirlist[DEATH_MATCH_EPISODE - 1].name );
        else strcat( text, "dm levels" );
        strcpy(multip_opt_text[2] ,text );
        strcpy( text, "level: " );
        if ( KILLING_MODE == DEATHMATCH )
            strcat( text, DEATH_MATCH_LEVEL );
        strcpy(multip_opt_text[3] , text );
        strcpy( text, "game type: " );
        if ( GAME_MODE == SPLIT_SCREEN )
            strcat( text, "split screen" );
        if ( GAME_MODE == NETWORK )
            strcat( text, "network" );
        strcpy(multip_opt_text[4], text );
        strcpy( text, "game mode: " );
        if ( KILLING_MODE == COOPERATIVE )
            strcat( text, "cooperative" );
        if ( KILLING_MODE == DEATHMATCH )
            strcat( text, "deathmatch" );
        strcpy(multip_opt_text[5],text );
        for ( a = 0; a < num_multip_options; a ++  )
        {
            if ( a == selected )
            {
                FONT_NUM = 0;
                writefonts2( 45, ( 100 - yoffs )  + ( a*15 ), multip_opt_text[a], 1 );
            }
            else
            {
                FONT_NUM = 2;
                writefonts2( 45, ( 100 - yoffs )  + ( a*15 ), multip_opt_text[a], 1 );
            }
        }
        if ( first == 1 )
        {
            fadein( virbuff, pal ); first = 0;
        }
        memcpy( screen, virbuff, 64000 );
        while ( !k::state(k::ESC)&&!k::state(k::ARROW_UP)&&!k::state(k::ARROW_DOWN)&&!k::state(k::ENTER) )
        {
            if ( clock() != oclock )
            {
                oclock = clock();
                draw_ball( 20, ( 98 - yoffs )  + ( selected*15 ), cnt );
                cnt ++;
                if ( cnt > 23 ) cnt = 0;
            }
            tk_port::event_tick();
        };
        MIDASplaySample( samplep[KLIKWAV], MIDAS_CHANNEL_AUTO, 0, 22500, EFFECT_VOLUME, MIDAS_PAN_MIDDLE );
        if ( k::state(k::ARROW_UP) )
        {
            selected --;if ( selected < 0 ) selected = num_multip_options - 1; k::clear(k::ARROW_UP);
        }
        if ( k::state(k::ARROW_DOWN) )
        {
            selected ++;if ( selected >= num_multip_options ) selected = 0; k::clear(k::ARROW_DOWN);
        }
        if ( k::state(k::ENTER) )
        {
            k::clear(k::ENTER);
            if ( selected == 0 )
            {
                if ( KILLING_MODE == DEATHMATCH )
                {
                    if ( GAME_MODE == NETWORK )
                    {
                        if ( deathmatch_options())
                        {
                            NETWORK_MODE = SERVER;
                            setup_ipx();
                            game();
                            desetup_ipx();
                            quit = 1;
                            dontfade = 1;
                        }
                        else multip_options_scr( num_multip_options, "EFPS/COOL.EFP" );
                    }
                    if ( GAME_MODE == SPLIT_SCREEN )
                    {
                        if ( deathmatch_options())
                        {
                            game();
                            quit = 1;
                            dontfade = 1;
                        }
                        else multip_options_scr( num_multip_options, "EFPS/COOL.EFP" );
                    }
                }
                else
                {
                    if ( GAME_MODE == NETWORK )
                    {
                        NETWORK_MODE = SERVER;
                        setup_ipx();
                        game();
                        desetup_ipx();
                        quit = 1;
                        dontfade = 1;
                    }
                    if ( GAME_MODE == SPLIT_SCREEN )
                    {
                        save_options();
                        game();
                        quit = 1;
                        dontfade = 1;
                    }
                }
            }
            if ( selected == 1 )
            {

                GAME_MODE = NETWORK;
                NETWORK_MODE = CLIENT;

                k::getch();

                setup_ipx();
                save_options();
                if (select_server()) game();
                desetup_ipx();

                quit = 1;
                dontfade = 1;

            }
            if ( selected == 2 )
            {
                DEATH_MATCH_EPISODE ++;if ( DEATH_MATCH_EPISODE > dirs ) DEATH_MATCH_EPISODE = 0; strcpy( DEATH_MATCH_LEVEL, filelist[DEATH_MATCH_EPISODE][0].name );
            }
            if ( selected == 3 ) if ( KILLING_MODE == DEATHMATCH )
            {
                choose_load_file( DEATH_MATCH_LEVEL, DEATH_MATCH_EPISODE ); multip_options_scr( num_multip_options, "EFPS/COOL.EFP" );
            }
            if ( selected == 4 )
            {
                if (GAME_MODE==SPLIT_SCREEN)
                {
                    if (net_avail)
                    {
                        GAME_MODE = NETWORK;

                        // Only deathmatch is supported over network
                        KILLING_MODE = DEATHMATCH;
                        saved_killing_mode = KILLING_MODE;
                    }
                }
                else GAME_MODE=SPLIT_SCREEN;
             saved_game_mode=GAME_MODE;
            }
            if ( selected == 5 )
            {
                if ( GAME_MODE != NETWORK )
                {
                    if ( ++KILLING_MODE > 1 ) KILLING_MODE = 0;
                }
                else
                {
                    // Only deathmatch is supported over network
                    KILLING_MODE = DEATHMATCH;
                }

                saved_killing_mode = KILLING_MODE;
            }
            if ( selected == 6 )
            {
                quit = 1;
            }
        }
        if ( k::state(k::ESC) )
        {
            quit = 1;k::clear(k::ESC);
        }
    }
    if ( !dontfade ) fadeout( virbuff, pal );
    save_options();
}

void options()
{
    int yoffs = ( 15*num_options )  / 2, a, cnt = 0, selected = 0, quit = 0, oclock = 0;
    fadeout( virbuff, pal );
    options_scr( num_options, "EFPS/COOL.EFP" );
    k::clear(k::ENTER);
    while( !quit )
    {
        memcpy( virbuff, picture, 64000 );
        for ( a = 0; a < num_options; a ++  )
        {
            if ( a == selected )
            {
                FONT_NUM = 0;
                writefonts2( 160 - ( str_length( opt_text[a] )  / 2 ), ( 100 - yoffs )  + ( a*15 ), opt_text[a], 1 );
            }
            else
            {
                FONT_NUM = 2;
                writefonts2( 160 - ( str_length( opt_text[a] )  / 2 ), ( 100 - yoffs )  + ( a*15 ), opt_text[a], M_RED );
            }
        }
        if ( first == 1 )
        {
            fadein( virbuff, pal ); first = 0;
        }
        memcpy( screen, virbuff, 64000 );
        while (!(quit || k::state(k::ESC) || k::state(k::ARROW_UP) || k::state(k::ARROW_DOWN) || k::state(k::ENTER)))
        {
            if ( clock() != oclock )
            {
                oclock = clock();
                draw_ball( 50, ( 98 - yoffs )  + ( selected*15 ), cnt );
                draw_ball( 320 - 70, ( 98 - yoffs )  + ( selected*15 ), cnt );
                cnt ++;
                if ( cnt > 23 ) cnt = 0;
            }
            tk_port::event_tick();
        };
        MIDASplaySample( samplep[KLIKWAV], MIDAS_CHANNEL_AUTO, 0, 22500, EFFECT_VOLUME, MIDAS_PAN_MIDDLE );
        if ( k::state(k::ARROW_UP) )
        {
            selected --;if ( selected < 0 ) selected = num_options - 1; k::clear(k::ARROW_UP);
        }
        if ( k::state(k::ARROW_DOWN) )
        {
            selected ++;if ( selected >= num_options ) selected = 0; k::clear(k::ARROW_DOWN);
        }
        if ( k::state(k::ENTER) )
        {
            if ( selected == 0 )
            {
                player_options(); options_scr( num_options, "EFPS/COOL.EFP" );
            }
            if ( selected == 1 )
            {
                game_options(); options_scr( num_options, "EFPS/COOL.EFP" );
            }
            if ( selected == 2 )
            {
                sound_options(); options_scr( num_options, "EFPS/COOL.EFP" );
            }
            if ( selected == 3 )
            {
                quit = 1;
            }
        }
        if ( k::state(k::ESC) )
        {
            quit = 1;k::clear(k::ESC);
        }
    }
    save_options();
    fadeout( virbuff, pal );
}
