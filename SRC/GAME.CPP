#include <cstdarg>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iostream>

#include "INPUT/KEYB.H"
#include "GRAPHICS.H"
#include "DEFINES.H"
#include "ERROR/ERROR.H"
#include "EFP/EFP.H"
#include "GLOBVAR.H"

/*************************NET STUFF **************************************/
#include "NET/NET.H"
#include "NET/NETDEFS.H"

int ipxofs[MAX_PLAYERS];
char ipxstream[MAX_PLAYERS][MAXDATASIZE];
struct nodeaddr serveraddr;
char servername[10];
struct packet *snd, *rec[RECEIVERS];

/************************* NET STUFF **************************************/
#include "MISCFUNC.H"
#include "WRITE.H"// tosi paskoja rutiineja !!!
#include "CLASSES.H"
#include "ROUT.H"
#include "FADE.H"
#include "DRAW.H"
#include "FUNCS.H"
#include "SHOP.H"
#include "NEWFUNCS.H"
#include "OPTIONS.H"
#include "I_FUNCS.H" // paska funkkareita
#include "MAKET_IN.H"

void menu_biisi()
{
    if ( ph ) MIDASstopModule( ph );
    ph = MIDASplayModule( biisi[0], TRUE );
    MIDASsetMusicVolume( ph, MUSIC_VOLUME );
}

void peli_biisi()
{
    static int biisi_count=0;

    if ( ph ) MIDASstopModule( ph );
    if (biisi[biisi_count+1]==NULL) {
        warn("peli_biisi():biisi[biisi_count+1]==NULL!!!\nbiisi_count: %d\n", biisi_count);
        return;
    }
    ph = MIDASplayModule( biisi[biisi_count+1], TRUE );
    MIDASsetMusicVolume( ph, MUSIC_VOLUME );
    biisi_count++;
    biisi_count%= 5;
}

void prevr( void )
{
    framecount++;
}

void make_floors()
{
    int a, lum;
    char col;
    for ( a = 0; a < 64000; a++ )
    {
        lum = ( int ) ( ( float ) ( pal[floorspr[a]*3]*0.2 )  + ( float ) ( pal[floorspr[a]*3 + 1]*0.5 )  + ( float ) ( pal[floorspr[a]*3 + 2]*0.3 )  )  >> 2;
        col = ( char ) ( ( 208 + 15 )  - lum );
        floorspr2[a] = col;
    }
}

/*
void load_tables()
{
    FILE *tab;
    tab = fopen( "PALETTE.TAB", "rb" );
    fread( trans_table, sizeof( trans_table ), 1, tab );
    fread( shadow_table, sizeof( shadow_table ), 1, tab );
    fread( &normal_l, sizeof( normal_l ), 1, tab );
    fread( &red_l, sizeof( red_l ), 1, tab );
    fread( &yellow_l, sizeof( yellow_l ), 1, tab );
    fread( &explo_l, sizeof( explo_l ), 1, tab );
    fclose( tab );
}
*/

void alloc_net_mem()
{
    int a;
    snd = ( struct packet * ) malloc( sizeof( struct packet ) );
    if ( snd == NULL )
    {
        std::cout << "Can't alloc lowmem!\n" << std::endl;
        net_avail = 0;
        return;
    }
    for ( a = 0; a < RECEIVERS; a ++  )
    {
        rec[a] = ( struct packet * ) malloc( sizeof( struct packet ) );
        if ( rec[a] == NULL )
        {
            std::cout << "Can't alloc lowmem!\n"<< std::endl;
            net_avail = 0;
            return;
        }
    }
}

void free_net_mem()
{
    int a;
    if ( net_avail )
    {
        free( snd );
        for ( a = 0; a < RECEIVERS; a ++  )
        {
            free( rec[a] );
        }
    }
}

void do_all()
{
    int a;
    char text[30];
    std::cout << "\n                ================== Starting TK =================\n\n";
    if ( tk_port::net::init()  )
    {
        std::cout << "TCP/IP stack initialized\n";
        net_avail = 1;
        alloc_net_mem();
    }
    else std::cout << "IPX _NOT_ detected!\n";
    if ( MIDASstartup()  )
    {
        if ( MIDASinit()  )
        {
            std::cout << "MIDAS initialized\n";
            if ( MIDASsetTimerCallbacks( target_frames*1000, FALSE, &prevr, NULL, NULL )  ) std::cout << "Timer synchronized\n";
        }
        else error( "Midas initialization failed\n" );
        if (!MIDASopenChannels( 20 )) error( "Error opening sound channels\n" );
        if (!MIDASallocAutoEffectChannels( 4 )) error( "Error allocating effect channels\n" );
        std::cout << "Loading sounds";fflush(stdout);
        samplep[MAGNUMWAV] = MIDASloadWaveSample( "WAVS/MAGNUM.WAV", FALSE );putchar('.');
        samplep[PISTOLWAV] = MIDASloadWaveSample( "WAVS/PISTOL.WAV", FALSE );putchar('.');
        samplep[SHOTGUNWAV] = MIDASloadWaveSample( "WAVS/RIFLE.WAV", FALSE );putchar('.');
        samplep[EXPLOWAV] = MIDASloadWaveSample( "WAVS/EXPLO.WAV", FALSE );putchar('.');
        samplep[DEATHWAV] = MIDASloadWaveSample( "WAVS/DEATH.WAV", FALSE );putchar('.');
        samplep[KLIKWAV] = MIDASloadWaveSample( "WAVS/KLIK.WAV", FALSE );putchar('.');
        samplep[HITWAV] = MIDASloadWaveSample( "WAVS/HIT.WAV", FALSE );putchar('.');
        samplep[SINKOWAV] = MIDASloadWaveSample( "WAVS/SINKO.WAV", FALSE );putchar('.');
        samplep[WOOSHWAV] = MIDASloadWaveSample( "WAVS/WOOSH.WAV", FALSE );putchar('.');
        samplep[FLAMEWAV] = MIDASloadWaveSample( "WAVS/FLAME.WAV", FALSE );putchar('.');
        std::cout << "\nLoading musics";fflush(stdout);
        biisi[0] = MIDASloadModule("MUSIC/MENU.S3M");putchar('.');
        biisi[1] = MIDASloadModule("MUSIC/BACK.S3M");putchar('.');
        biisi[2] = MIDASloadModule("MUSIC/MIELIP2.S3M");putchar('.');
        biisi[3] = MIDASloadModule("MUSIC/MIELIP2.S3M");putchar('.');
        biisi[4] = MIDASloadModule("MUSIC/BACK.S3M");putchar('.');
        biisi[5] = MIDASloadModule("MUSIC/BACK.S3M");putchar('.');
//      biisi[2] = MIDASloadModule("MUSIC/BACK.S3M");putchar('.');
//      biisi[3] = MIDASloadModule("MUSIC/UNIC_3X.S3M");putchar('.');
//      biisi[4] = MIDASloadModule("MUSIC/MIELIP2.S3M");putchar('.');
//      biisi[5] = MIDASloadModule("MUSIC/UNIC_2X.S3M");putchar('.');
//      biisi[1] = MIDASloadModule("MUSIC/UNIC_1X.S3M");putchar('.');
        /* TODO SOUND
        for (a=0;a<6;a++)
         if (biisi[a]==NULL) error("Musicmodule #%d NULL!\n",a);
        */
        std::cout << std::endl;
    }
    else
    {
        warn("Could not start MIDAS, starting without sounds\n");
    }
    i::init( 0 );
    std::cout << "Keyboard handler initialized\n";
    std::cout << "Loading fonts";
    FONT_NUM = 0; loadfonts( "FNTS/BLOODY.FNT" );   putchar('.');
    FONT_NUM = 1; loadfonts( "FNTS/8X8B.FNT" );     putchar('.');
    FONT_NUM = 2; loadfonts( "FNTS/BLOODY2.FNT" );   putchar('.');
    FONT_NUM = 3; loadfonts( "FNTS/SNUMBERS.FNT" ); putchar('.');
    std::cout << "\nLoading bitmaps";
    load_efp( "EFPS/FLOOR1.EFP", floorspr, 0 );  putchar('.');
    load_efp( "EFPS/WALLS1.EFP", wallspr, 0 );       putchar('.');
    load_efp( "EFPS/ENEMY0.EFP", enemyspr0, 0 );     putchar('.');
    load_efp( "EFPS/ENEMY1.EFP", enemyspr1, 0 );     putchar('.');
    load_efp( "EFPS/ENEMY2.EFP", enemyspr2, 0 );     putchar('.');
    load_efp( "EFPS/ENEMY3.EFP", enemyspr3, 0 );     putchar('.');
    load_efp( "EFPS/ENEMY4.EFP", enemyspr4, 0 );     putchar('.');
    load_efp( "EFPS/ENEMY5.EFP", enemyspr5, 0 );     putchar('.');
    load_efp( "EFPS/ENEMY6.EFP", enemyspr6, 0 );     putchar('.');
    load_efp( "EFPS/ENEMY7.EFP", enemyspr7, 0 );     putchar('.');
    load_efp( "EFPS/RAMBO2.EFP", rambospr, 0 );  putchar('.');
    load_efp( "EFPS/BODYP.EFP", bodypartspr, 0 );    putchar('.');
    load_efp( "EFPS/EFFECTS.EFP", effectspr, 0 );    putchar('.');
    load_efp( "EFPS/WARE.EFP", warespr, 0 );         putchar('.');
    load_efp( "EFPS/TARGET.EFP", targetspr, 0 );     putchar('.');
    load_efp( "EFPS/C4.EFP", c4spr, 0 );             putchar('.');
    load_efp( "EFPS/MINE.EFP", minespr, 0 );         putchar('.');
    load_efp( "EFPS/BOMB.EFP", grenadespr, 0 );  putchar('.');
    load_efp( "EFPS/SHADOWS.EFP", shadowspr, 0 );    putchar('.');
    load_efp( "EFPS/CRATES.EFP", cratespr, 0 );  putchar('.');
    pre_count();
    CalCuLaTE_Sine_Table_AND_Be_Happy();// for fade
    std::cout << "\nSine and Cosine tables ready\n";
    for ( a = 0; a < LIGHTS; a ++  )
        light[a].make( Light_sizes[a] );
    std::cout << "Lights mapped\n";
    make_light_tables();
    std::cout << "Palette tables calculated \n";
    load_efp_pal( "EFPS/WALLS1.EFP", pal );
    std::cout << "Calculating dark floor... ";
    make_floors();
    std::cout << "OK \n";
    load_options();std::cout << "Options loaded\n";
    list_files( &dirs, dirlist, "LEVS/*", _A_SUBDIR );
    std::cout << dirs << " episodes found\n";
    list_files( &files_in_episode[0], filelist[0], "LEVS/*.LEV", _A_NORMAL );
    for ( a = 0; a < dirs && a < 256; a++ )
    {
        strcpy( text, "LEVS/" );
        strcat( text, dirlist[a].name );
        strcat( text, "/*.LEV" );
        list_files( &files_in_episode[a + 1], filelist[a + 1], text, _A_NORMAL );
    }
    define_bullet_types();
    define_effects();
}

void free_all()
{
    MIDASclose();
    free_net_mem();
    tk_port::net::deinit();
    i::deinit();
}

int check_place( int x, int y, int size )
{
    int a, ret = 0;

    // Check that the object fits to level vertically...
    if ( ( ( y + size ) / 20 ) >= level_y_size ) return 1;
    // ... and horizontally
    if ( ( ( x + size ) / 20 ) >= level_x_size ) return 1;

    if ( level[( y / 20 ) *level_x_size + ( x / 20 ) ].type!= FLOOR ) ret = 1;
    if ( level[( ( y + size )  / 20 ) *level_x_size + ( x / 20 ) ].type!= FLOOR ) ret = 1;
    if ( level[( y / 20 ) *level_x_size + ( ( x + size )  / 20 ) ].type!= FLOOR ) ret = 1;
    if ( level[( ( y + size )  / 20 ) *level_x_size + ( ( x + size )  / 20 ) ].type!= FLOOR ) ret = 1;
    for ( a = 0; a < ACTIVE_PLAYERS; a ++ )
        if ( get_dist( x, y, aplayer[a]->x, aplayer[a]->y ) < 50 ) ret = 1;
    return (ret);
}

void place_enemies()

{
    int a, done, nx = 0, ny = 0, b, c;
    for ( a = 0; a < ENEMIES; a ++  )
    {
        done = 0;
        while ( done == 0 )
        {
            nx = ( rand()  % ( level_x_size - 2 )  )  + 1;
            ny = ( rand()  % ( level_y_size - 2 )  )  + 1;
            if ( level[ny*level_x_size + nx].type == FLOOR )
            {
                done = 1;
                if ( a > 0 )
                    for ( b = 0; b < a; b++ )
                        if ( nx == enemy[b].x && ny == enemy[b].y ) done = 0;
                for ( c = 0; c < ACTIVE_PLAYERS && done == 1; c++ )
                    if ( get_dist( nx, ny, pl_start_x[c], pl_start_y[c] ) < 8 ) done = 0;
            }
        }
        enemy[a].x = nx;
        enemy[a].y = ny;
    }
    for ( a = 0; a < ENEMIES; a ++  )
    {
        enemy[a].x = ( ( enemy[a].x*20 )  - 5 );
        enemy[a].y = ( ( enemy[a].y*20 )  - 5 );
        enemy[a].angle = rand()  % 360;
    }
}

void place_normal_game_crates()
{
    int a, b, x = 0, y = 0, crat = 0, bullets, weapons;
    if ( leveldata.version == 1 )
    {
        weapons = DIFF_WEAPONS - 2;
        bullets = DIFF_BULLETS - 2;
    }
    else if ( leveldata.version == 2 )
    {
        weapons = DIFF_WEAPONS - 1;
        bullets = DIFF_BULLETS - 1;
    }
    else // used to be: if ( leveldata.version == 3 || leveldata.version >= 4)
    {
        weapons = DIFF_WEAPONS;
        bullets = DIFF_BULLETS;
    }
    for ( a = 0; a < weapons; a++ )
        for ( b = 0; b < normal_game_level_info.weapon_crates[a]; b++, crat++ )
            if ( crat < MAX_CRATES )
            {
                x = 0;
                y = 0;
                crate[crat].type = 0;
                crate[crat].weapon = a;
                while (check_place( x, y, 14 ))
                    if ( check_place( x, y, 14 ))
                    {
                        x = rand() % (level_x_size * 20);
                        y = rand() % (level_y_size * 20);
                    }
                crate[crat].x = x;
                crate[crat].y = y;
                crate[crat].IN_USE = 1;
            }
    for ( a = 0; a < bullets; a++ )
        for ( b = 0; b < normal_game_level_info.bullet_crates[a]; b++, crat++ )
            if ( crat < MAX_CRATES )
            {
                x = 0;
                y = 0;
                crate[crat].type = 1;
                crate[crat].bullet = a;
                while (check_place( x, y, 14 ))
                    if ( check_place( x, y, 14 ))
                    {
                        x = rand() % (level_x_size * 20);
                        y = rand() % (level_y_size * 20);
                    }
                crate[crat].x = x;
                crate[crat].y = y;
                crate[crat].IN_USE = 1;
            }
    for ( b = 0; b < normal_game_level_info.energy_crates; b++, crat++ )
        if ( crat < MAX_CRATES )
        {
            x = 0;
            y = 0;
            crate[crat].type = 2;
            crate[crat].energy = 1;
            while (check_place( x, y, 14 ))
                if ( check_place( x, y, 14 ))
                {
                    x = rand() % (level_x_size * 20);
                    y = rand() % (level_y_size * 20);
                }
            crate[crat].x = x;
            crate[crat].y = y;
            crate[crat].IN_USE = 1;
        }
    if ( leveldata.version >= 5 )
        for ( a = 0; a < normal_crate_amount && crat < MAX_CRATES; a++, crat++ )
        {
            crate[crat].x = normal_crate_info[a].x;
            crate[crat].y = normal_crate_info[a].y;
            crate[crat].type = normal_crate_info[a].type1;
            if ( crate[crat].type == 0 )
                crate[crat].weapon = normal_crate_info[a].type2;
            if ( crate[crat].type == 1 )
                crate[crat].bullet = normal_crate_info[a].type2;
            if ( crate[crat].type == 2 )
                crate[crat].energy = 1;
            crate[crat].IN_USE = 1;
        }
}

void place_death_match_crates()
{
    int a, b, x = 0, y = 0, crat = 0, weapons, bullets;
    if ( leveldata.version == 1 )
    {
        weapons = DIFF_WEAPONS - 2;
        bullets = DIFF_BULLETS - 2;
    }
    else if ( leveldata.version == 2 )
    {
        weapons = DIFF_WEAPONS - 1;
        bullets = DIFF_BULLETS - 1;
    }
    else // Used to be: if ( leveldata.version == 3 || leveldata.version >= 4)
    {
        weapons = DIFF_WEAPONS;
        bullets = DIFF_BULLETS;
    }
    for ( a = 0; a < weapons; a++ )
        for ( b = 0; b < death_match_level_info.weapon_crates[a]; b++, crat++ )
            if ( crat < MAX_CRATES )
            {
                x = 0;
                y = 0;
                crate[crat].type = 0;
                crate[crat].weapon = a;
                while (check_place( x, y, 14 ))
                    if ( check_place( x, y, 14 ))
                    {
                        x = rand() % (level_x_size * 20);
                        y = rand() % (level_y_size * 20);
                    }
                crate[crat].x = x;
                crate[crat].y = y;
                crate[crat].IN_USE = 1;
            }
    for ( a = 0; a < bullets; a++ )
        for ( b = 0; b < death_match_level_info.bullet_crates[a]; b++, crat++ )
            if ( crat < MAX_CRATES )
            {
                x = 0;
                y = 0;
                crate[crat].type = 1;
                crate[crat].bullet = a;
                while (check_place( x, y, 14 ))
                    if ( check_place( x, y, 14 ))
                    {
                        x = rand() % (level_x_size * 20);
                        y = rand() % (level_y_size * 20);
                    }
                crate[crat].x = x;
                crate[crat].y = y;
                crate[crat].IN_USE = 1;
            }
    for ( b = 0; b < death_match_level_info.energy_crates; b++, crat++ )
        if ( crat < MAX_CRATES )
        {
            x = 0;
            y = 0;
            crate[crat].type = 2;
            crate[crat].energy = 1;
            while (check_place( x, y, 14 ))
                if ( check_place( x, y, 14 ))
                {
                    x = rand() % (level_x_size * 20);
                    y = rand() % (level_y_size * 20);
                }
            crate[crat].x = x;
            crate[crat].y = y;
            crate[crat].IN_USE = 1;
        }

    if ( leveldata.version >= 5 )
        for ( a = 0; a < deathmatch_crate_amount && crat < MAX_CRATES; a++, crat++ )
        {
            crate[crat].x = deathmatch_crate_info[a].x;
            crate[crat].y = deathmatch_crate_info[a].y;
            crate[crat].type = deathmatch_crate_info[a].type1;
            if ( crate[crat].type == 0 )
                crate[crat].weapon = deathmatch_crate_info[a].type2;
            if ( crate[crat].type == 1 )
                crate[crat].bullet = deathmatch_crate_info[a].type2;
            if ( crate[crat].type == 2 )
                crate[crat].energy = 1;
            crate[crat].IN_USE = 1;
        }
}

void alusta_enemies()
{
    int a, b, c, d;
    ENEMIES = 0;
    c = 0;
    for ( b = 0; b < DIFF_ENEMIES; b++ )
    {
        ENEMIES += general_level_info.enemies[b];
        for ( a = 0; a < general_level_info.enemies[b] && c < MAX_ENEMIES; a++ )
        {
            enemy[c].PUSH_POWER = 0;
            enemy[c].burning = 0;
            enemy[c].rangle = 0;
            enemy[c].angle = 0;
            enemy[c].load_cnt = 0;
            enemy[c].WALK = 0;
            enemy[c].FIRE = 0;
            for ( d = 0; d < MAX_PLAYERS; d++ )
                enemy[c].SEE_PL[d] = 0;
            enemy[c].DEAD = 0;
            enemy[c].walk_cnt = 0;
            enemy[c].type = b;
            enemy[c].energy = enemy_info[enemy[c].type].energy;
            c++;
        }
    }
    if ( ENEMIES > MAX_ENEMIES ) ENEMIES = MAX_ENEMIES;
}

void null_effects()
{
    int a;
    for ( a = 0; a < MAX_EFFECTS; a ++  )
    effect[a].IN_USE = 0;
}

void null_crates()
{
    int a;
    for ( a = 0; a < MAX_CRATES; a ++  )
    {
        crate[a].text_count = 0;
        crate[a].weapon = 0;
        crate[a].bullet = 0;
        crate[a].wake_up_count = 0;
        crate[a].IN_USE = 0;
    }
}

void null_body_parts()
{
    int a;
    for ( a = 0; a < MAX_BODY_PARTS; a ++  )
    body_part[a].IN_USE = 0;
}

void null_bullets()
{
    int a;
    for ( a = 0; a < MAX_BULLETS; a ++  )
    bullet[a].IN_USE = 0;
}

void animate_players()
{
    int a;
    for ( a = 0; a < ACTIVE_PLAYERS; a ++ )
        aplayer[a]->animate();
}

int player_amount()
{
    int a,b=0;
    for (a=0;a<MAX_PLAYERS;a++)
     if (player[a].enabled) b++;
    return(b);
}

void animate_enemies()
{
    int a;
    for ( a = 0; a < ENEMIES; a ++ )
        enemy[a].animate();
}

void quiti()
{   const char *quitstr="REALLY WANNA QUIT ? (y)";
    int a, x = 160 - ( str_length( quitstr )  / 2 );
    clock_t time;
    for ( a = 0; a < 12; a++ )
        memset( virbuff + (a + (scr_y_size / 2) - 6) * 320 + x - 2, 0, 320 - (x * 2) + 4 );
    writefonts( x, (scr_y_size / 2) - 4, quitstr, 1 );
    memcpy( screen, virbuff, 64000 );
    i::clear_stack();
    time = clock();
    if ( i::wait_for_keypress() == k::CHR_Y ) quit = 1;

    st += clock()  - time;
}

void screen_capture()
{
    FILE *sci;
    int free = 0, num = 0;
    char text1[19], text2[13];
    while ( free == 0 && num <= 99 )
    {
        strcpy( text1, "screen" );
        sprintf( text2, "%d", num );
        strcat( text1, text2 );
        strcat( text1, ".bmp" );
        if ( ( sci = fopen( text1, "rb" )  )  == NULL ) free = 1;else fclose( sci );
        num ++;
    }
    tk_port::save_screenshot( text1 );
    strncat( text1, " saved" , sizeof( text1 ) - strlen( text1 ) - 1 );
    message_board.add_message(text1);
}

void chkeys()
{
    int a;

    if ( i::state(k::ESC) ) quiti();

    for ( a = 0; a < ACTIVE_PLAYERS; a++ )
        if ( !aplayer[a]->DEAD )
            aplayer[a]->chk_keys( a );

    if ( i::state(k::SPACE) )
    {
        MAP_ON = !MAP_ON ? 1 : 0;
        i::clear(k::SPACE);
    }
    if ( i::state(k::F5) )
    {
        FRAMES_ON = FRAMES_ON == 0 ? 1 : 0;
        i::clear(k::F5);
    }
    if ( i::state(k::F12) )
    {
        screen_capture();
        i::clear(k::F12);
    }
}

void chk_wake_ups()
{
    for ( int a = 0; a < MAX_CRATES; a ++  )
        if ( crate[a].IN_USE == 0 )
            crate[a].chk_wake_up(a);
}

void move_crate_texts()
{
    for ( int a = 0; a < MAX_CRATES; a++ )
        if ( crate[a].IN_USE )
            crate[a].move_text();
}

void move_body_parts()
{
    for ( int a = 0; a < MAX_BODY_PARTS; a++ )
        if ( body_part[a].IN_USE )
            if ( body_part[a].speed > 0 )
            {
                body_part[a].move();
                body_part[a].speed -= 0.2f;
            }
}

void roll_enemies()
{
    for ( int a = 0; a < ENEMIES; a++ )
        if ( !enemy[a].DEAD )
        {
            if ( enemy[a].tangle == enemy[a].angle && enemy[a].walk_cnt == 0 && enemy[a].see_somebody == 0 )
                if ( rand() % 50 == 1 )
                    enemy[a].tangle = rand() % 360;
            enemy[a].rotate();
        }
}

void move_enemies()
{
    for ( int a = 0; a < ENEMIES; a++ )
    {
        if ( !enemy[a].DEAD )
        {
            int see = 0;
            for ( int b = 0; b < MAX_PLAYERS && see == 0; b++ )
                if ( player[b].enabled && enemy[a].SEE_PL[b] )
                    see = 1;
            if ( see == 0 )
                if ( enemy[a].walk_cnt == 0 )
                    if ( rand() % 100 == 1 ) enemy[a].walk_cnt = (rand() % 40) + 20;
            if ( enemy[a].walk_cnt > 0 )
            {
                enemy[a].move( enemy[a].rangle * 9, 0 );
                enemy[a].walk_cnt--;
                if ( enemy[a].WALK == 0 ) enemy[a].WALK = 1;
            }
            else enemy[a].WALK = 0;
        }
        if ( enemy[a].PUSH_POWER > 0 )
        {
            if ((int) enemy[a].PUSH_POWER > 0 )
                enemy[a].move( enemy[a].PUSH_ANGLE, (int) enemy[a].PUSH_POWER );
            enemy[a].PUSH_POWER -= 0.2f;
        }
    }
}

void push_players()
{
    for ( int a = 0; a < ACTIVE_PLAYERS; a++ )
        if ( aplayer[a]->PUSH_POWER > 0 )
        {
            if ((int) aplayer[a]->PUSH_POWER > 0 )
                aplayer[a]->move( aplayer[a]->PUSH_ANGLE, (int) aplayer[a]->PUSH_POWER, a );
            aplayer[a]->PUSH_POWER -= 0.2f;
        }
}

void roll_effects()
{
    for ( int a = 0; a < MAX_EFFECTS; a++ )
        effect[a].roll();
}

void move_bullets()
{
    for ( int a = 0; a < MAX_BULLETS; a++ )
        if ( bullet[a].IN_USE == 1 )
            bullet[a].move();
}

void get_angles()
{
    for ( int a = 0; a < ENEMIES; a++ )
        if ( !enemy[a].DEAD )
            enemy[a].get_angles();
}

void try_see_players()
{
    for ( int a = 0; a < ENEMIES; a++ )
        if ( !enemy[a].DEAD )
            enemy[a].try_see_players();
}

void enemies_load()
{
    for ( int a = 0; a < ENEMIES; a++ )
        if ( !enemy[a].DEAD )
            enemy[a].load();
}

void chk_for_shoot()
{
    for ( int a = 0; a < ENEMIES; a++ )
        if ( !enemy[a].DEAD )
            enemy[a].chk_for_shoot();
}

void complete()
{
    int a;
    next_level ++ ;
    a = ( next_level*5 )  % 360;
    writefonts( 160 + ( ( 160 - ( ( float ) next_level / ( ( float ) C_DELAY / 160 )  )  ) *sini[a] )  - 45, ( scr_y_size / 2 )  + ( 100 - ( ( float ) next_level / ( ( float ) C_DELAY / 100 )  )  ) *cosi[a], "MISSION COMPLETED", 3*16 );
}

void kello()
{
    char col = 1;
    if ( ! next_level )
        complete_time = (clock() - st) / CLOCKS_PER_SEC;
    if ( complete_time > general_level_info.time_limit * TIME_ADD ) col = 3 * 16;
    writefonts( 151, 10, c_2_c( complete_time ), col );
}

void frames_per_sec()
{
    char text[5];
    sprintf( text, "%d", F_PER_SEC );
    writefonts( 151, 20, text, 9*16 );
    sprintf( text, "%d", OVER_POWER  );
    writefonts( 151, 30, text, 7*16 );
}

void dec_hits()
{
    int a;
    for ( a = 0; a < ACTIVE_PLAYERS; a ++  )
    if ( aplayer[a]->HIT > 0 ) aplayer[a]->HIT --;
}

void are_all_dead()
{
    int a;
    if ( KILLING_MODE!= DEATHMATCH )
    {
        all_dead = 1;
        for ( a = 0; a < ACTIVE_PLAYERS; a ++  )
        if ( aplayer[a]->enabled)
        if ( aplayer[a]->DEAD <= 14 ) all_dead = 0;
    }
    if ( all_dead )
    complete_time = ( general_level_info.time_limit*TIME_ADD )  + 100;
}

void seeing()
{
    int a;
    for ( a = 0; a < ACTIVE_PLAYERS; a ++  )
    {
        memset( aplayer[a]->see_level, 0, level_x_size*level_y_size );
        aplayer[a]->see();
    }
}

void fire_lights()
{
    int a, xoffs, yoffs;
    if ( GAME_MODE != SPLIT_SCREEN )
        for ( a = 0; a < MAX_PLAYERS; a ++ )
            if ( player[a].enabled && weapon[player[a].curr_weapon].gun == 1 && player[a].FIRE )
            {
                xoffs = (sini[player[a].rangle2] * 10) + (sini[(int) ((player[a].rangle2) + 270) % 360] * 6);
                yoffs = (cosi[player[a].rangle2] * 10) + (cosi[(int) ((player[a].rangle2) + 270) % 360] * 6);
                draw_light( &normal_l, player[a].x + 14 + xoffs - aplayer[0]->scr_x,
                            player[a].y + 13 + yoffs - aplayer[0]->scr_y, 3, 2 - player[a].FIRE, 0 );
            }
    if ( GAME_MODE == SPLIT_SCREEN )
    {
        for ( a = 0; a < ACTIVE_PLAYERS; a ++ )
        {
            if ( weapon[player[a].curr_weapon].gun == 1 && player[a].FIRE )
            {
                xoffs = (sini[player[a].rangle2] * 10) + (sini[(int) ((player[a].rangle2) + 270) % 360] * 6);
                yoffs = (cosi[player[a].rangle2] * 10) + (cosi[(int) ((player[a].rangle2) + 270) % 360] * 6);
                draw_light( &normal_l, player[a].x + 14 + xoffs - aplayer[0]->scr_x,
                            player[a].y + 13 + yoffs - aplayer[0]->scr_y, 3, 2 - player[a].FIRE, 1 );
                draw_light( &normal_l, (player[a].x + 14 + xoffs - aplayer[1]->scr_x) + 160,
                            player[a].y + 13 + yoffs - aplayer[1]->scr_y, 3, 2 - player[a].FIRE, 2 );
            }
        }
    }
    if ( GAME_MODE != SPLIT_SCREEN )
        for ( a = 0; a < ENEMIES; a ++ )
            if ( ! enemy[a].DEAD && enemy[a].FIRE )
            {
                xoffs = (sini[(int) (enemy[a].rangle * 9)] * 10) +
                        (sini[(int) ((enemy[a].rangle * 9) + 270) % 360] * 3);
                yoffs = (cosi[(int) (enemy[a].rangle * 9)] * 10) +
                        (cosi[(int) ((enemy[a].rangle * 9) + 270) % 360] * 3);
                draw_light( &normal_l, enemy[a].x + 14 + xoffs - aplayer[0]->scr_x,
                            enemy[a].y + 13 + yoffs - aplayer[0]->scr_y, 3, 2 - enemy[a].FIRE, 0 );
            }
    if ( GAME_MODE == SPLIT_SCREEN )
        for ( a = 0; a < ENEMIES; a ++ )
            if ( ! enemy[a].DEAD && enemy[a].FIRE )
            {
                xoffs = (sini[(int) (enemy[a].rangle * 9)] * 10) +
                        (sini[(int) ((enemy[a].rangle * 9) + 270) % 360] * 3);
                yoffs = (cosi[(int) (enemy[a].rangle * 9)] * 10) +
                        (cosi[(int) ((enemy[a].rangle * 9) + 270) % 360] * 3);
                draw_light( &normal_l, enemy[a].x + 14 + xoffs - player[0].scr_x,
                            enemy[a].y + 13 + yoffs - player[0].scr_y, 3, 2 - enemy[a].FIRE, 1 );
                draw_light( &normal_l, (enemy[a].x + 14 + xoffs - player[1].scr_x) + 160,
                            enemy[a].y + 13 + yoffs - player[1].scr_y, 3, 2 - enemy[a].FIRE, 2 );
            }
}

void sendipx(int dest, int type, ...);

void chk_burn()
{
    int a,b;

    for ( a = 0; a < MAX_PLAYERS; a ++ )
        if ( player[a].enabled && ! player[a].DEAD && player[a].burning )
        {
            if ( a == aplayer[0]->tindex || (GAME_MODE == SPLIT_SCREEN && a == aplayer[1]->tindex))
            {
                player[a].burning --;
                player[a].get_damage( 0.4f, player[a].Frying_player );
            }

            if ( aplayer[0]->tindex == a && GAME_MODE == NETWORK )
            {
                if ( NETWORK_MODE == CLIENT )
                    sendipx( 0, IPX_BURNING, player[a].tindex, player[a].burning );
                else
                    for ( b = 1; b < MAX_PLAYERS; b ++ )
                        if ( player[b].enabled )
                            sendipx( b, IPX_BURNING, player[a].tindex, player[a].burning );
            }

            if ( Steam_count % 6 == 0 )
                new_effect( NULL, STEAM, player[a].x + 14, player[a].y + 14, 135, 1 );
        }
    for ( a = 0; a < ENEMIES; a ++ )
        if ( ! enemy[a].DEAD && enemy[a].burning )
        {
            enemy[a].burning --;
            enemy[a].get_damage( 0.4f, enemy[a].Frying_player );
            if ( Steam_count % 6 == 0 )
                new_effect( NULL, STEAM, enemy[a].x + 14, enemy[a].y + 14, 135, 1 );
        }
}

#define s_r_a 8
int steam_rates[s_r_a] =
{
    15, 10, 5, 5, 4, 4, 3, 3
};

void do_steam()
{
    int a, b, du = 0, yl1 = scr_y_size / 2, yl2 = scr_y_size*1.5;
    for ( a = 0; a < (int)Steam_amount; a ++ , du = 0 )
    {
        if ( steam[a].x > aplayer[0]->scr_x - 160 )
         if ( steam[a].x < aplayer[0]->scr_x + 480 )
          if ( steam[a].y > aplayer[0]->scr_y - yl1 )
           if ( steam[a].y < aplayer[0]->scr_y + yl2 )
            du = 1;
        if ( GAME_MODE == SPLIT_SCREEN )
         if ( steam[a].x > aplayer[1]->scr_x - 160 )
          if ( steam[a].x < aplayer[1]->scr_x + 480 )
           if ( steam[a].y > aplayer[1]->scr_y - yl1 )
            if ( steam[a].y < aplayer[1]->scr_y + yl2 )
             du = 1;
        if ( du )
        {
            if ( steam[a].speed < s_r_a ) b = steam_rates[steam[a].speed];
            else b = 1;
            if ( Steam_count % b == 0 )
            new_effect( NULL, STEAM, steam[a].x, steam[a].y, steam[a].angle, steam[a].speed );
        }
    }
}

void game_shit()
{
    int a;
    chkeys();
    push_players();
    chk_burn();
    move_bullets();
    move_body_parts();
    move_crate_texts();
    if ( KILLING_MODE == DEATHMATCH )
     if ( (GAME_MODE != NETWORK) || (NETWORK_MODE==SERVER)) chk_wake_ups();
    for ( a = 0; a < ACTIVE_PLAYERS; a ++  )
     aplayer[a]->move_scr();
    if ( DARK_MODE ) seeing();

    // ENEMIES_ON_GAME is a UI option visible only for deathmatch games
    if ( ( KILLING_MODE != DEATHMATCH || ENEMIES_ON_GAME ) &&
        ( GAME_MODE != NETWORK || NETWORK_MODE == SERVER ) )
    {
     roll_enemies();
     get_angles();
     try_see_players();
     move_enemies();
     chk_for_shoot();
     enemies_load();
     animate_enemies();
    }
    draw_floor();
    for ( a = 0; a < ACTIVE_PLAYERS; a++ ) aplayer[a]->load();
    animate_players();
    draw_players( 0 );// DEad players
    draw_effects( 0 );
    draw_body_parts();
    if ( SHADOWS ) draw_gae_shadows();
    draw_crates();
    draw_bullets();
    if ( LIGHT_EFFECTS ) fire_lights();
    draw_live_enemies();
    draw_players( 1 );// Live players
    if ( SMOKE_EFFECTS ) do_steam();
    draw_walls();
    if ( SHADOWS ) draw_shadows();
    draw_effects( 1 );
    if ( LIGHT_EFFECTS ) draw_spots();
    draw_targets();
    if ( DARK_MODE ) draw_black();
    dec_hits();
    draw_hits();
    if ( MAP_ON ) map();
    draw_crate_texts();
    if ( FRAMES_ON ) frames_per_sec();
    if ( KILLING_MODE == DEATHMATCH ) draw_kills();
    draw_infos();
    draw_energies();
    message_board.draw();
    if ( next_level ) complete();
    if ( KILLING_MODE!= DEATHMATCH )
    kello();
    if ( first == 1 )
     {
       if (! (GAME_MODE==NETWORK && NETWORK_MODE==CLIENT))
        fadein( virbuff, pal ); else setpal(pal,256);
        first = 0;
     }
    memcpy( screen, virbuff, 320*scr_y_size );
    roll_effects();
    are_all_dead();
}

void do_prices()
{
    int a;
    for ( a = 0; a < DIFF_WEAPONS; a ++  )
        sell_price[a] = ( 0.8*weapon[a + 1].cost )  - ( rand()  % ( weapon[a + 1].cost / 2 )  );
    sell_price[DIFF_WEAPONS] = ( 0.8*shield_data.cost )  - ( rand()  % ( shield_data.cost / 2 )  );
    sell_price[DIFF_WEAPONS + 1] = ( 0.8*target_cost )  - ( rand()  % ( target_cost / 2 )  );
}

void victory()
{
    int a = 0, oclock;
    load_efp( "EFPS/SUNSET.EFP", picture, 0 );
    load_efp_pal( "EFPS/SUNSET.EFP", pal );
    setpal( pal, 0 );
    memcpy( screen, picture, 64000 );
    oclock = clock();
    while (a < 255)
        if ( oclock != clock())
        {
            oclock = clock();
            a += 10;
            setpal( pal, a );
        }
    i::clear(k::ESC);
    while (! i::state(k::ESC)) tk_port::event_tick();
    while (a > 0)
        if ( oclock != clock())
        {
            oclock = clock();
            a -= 10;
            setpal( pal, a );
        }
}

void flushipx()
{
    int a,b;
    char jee[100];
    for ( a = 0; a < MAX_PLAYERS; a++  )
    if (player[a].enabled)
    {
    sprintf(jee,"streami[%d][%d]",a,ipxofs[a]);
//  message_board.add_message(jee);
     if (ipxofs[a]!=0)
      {
        ipxstream[a][ ipxofs[a]++ ] = IPX_END;
        for (b=0;b<ipxofs[a];b++)
         snd->data[b]=ipxstream[a][b];
        tk_port::net::sendpacket( player[a].node, snd, (word)(ipxofs[a]+1) );
 //         RESET
        ipxofs[a] = 0;
      }
    }
//  message_board.add_message("flushed...");
}

void docheckflush()
{
    int a,b;
    char jee[100];

    for ( a = 0; a < MAX_PLAYERS; a++  )
    if (player[a].enabled)
    {
    sprintf(jee,"streami[%d][%d]",a,ipxofs[a]);
//  message_board.add_message(jee);
    if (ipxofs[a]>(MAXDATASIZE-100))
    {
        ipxstream[a][ ipxofs[a]++ ] = IPX_END;
        for (b=0;b<ipxofs[a];b++)
         snd->data[b]=ipxstream[a][b];
        tk_port::net::sendpacket( player[a].node, snd, (word)(ipxofs[a]+1) );
 //         RESET
        ipxofs[a] = 0;
    }
    }
//  message_board.add_message("flushed2...");
}

static void serialize_player_data( int player_index, int* data_index, char* buffer )
{
    unsigned int a;
    char *str;
    buffer[(*data_index)++] = (char) player_index;
    str = (char *)&player[player_index].x;
    for (a = 0;a < sizeof(float);a++) buffer[(*data_index)++] = *(str++);
    str = (char *)&player[player_index].y;
    for (a = 0;a < sizeof(float);a++) buffer[(*data_index)++] = *(str++);
    str = (char *)&player[player_index].rangle;
    for (a = 0;a < sizeof(int);a++) buffer[(*data_index)++] = *(str++);
    buffer[(*data_index)++] = (char)player[player_index].color;
    str = (char *)&player[player_index].burning;
    for (a = 0;a < sizeof(int);a++) buffer[(*data_index)++] = *(str++);
    str = (char *)&player[player_index].node;
    for (a = 0;a < sizeof(struct nodeaddr);a++) buffer[(*data_index)++] = *(str++);
    for (a = 0;a < 10;a++) buffer[(*data_index)++] = player[player_index].name[a];
    str = (char *)&player[player_index].ANIM;
    for (a = 0;a < sizeof(int);a++) buffer[(*data_index)++] = *(str++);
    str = (char *)&player[player_index].player_kills;
    for (a = 0;a < sizeof(int);a++) buffer[(*data_index)++] = *(str++);
    str = (char *)&player[player_index].DEAD;
    for (a = 0;a < sizeof(int);a++) buffer[(*data_index)++] = *(str++);
}

void sendipxnow( struct nodeaddr *node, int type, ... )
{
    int a = 0;
//  TEMP VARIABLES
    int b,c,d;
    char *str;
    char *p;

//  TEMP VARIABLES END
    va_list ap;
    va_start( ap, type );

    switch( type )
    {
        case IPX_GAMEDATA:    snd->data[a++] = ( char ) type;
                              b = va_arg( ap, int);
                           //   for (c=0;c<6;c++)
                           //    player[b].ipxnode.node[c]=node->node[c];
                              snd->data[a++] = ( char ) player_amount()+1;
                              snd->data[a++] = ( char ) b;
                              snd->data[a++] = ( char ) DEATH_MATCH_SPEED;
                              for (d=0;d<MAX_PLAYERS;d++)
                              if ((player[d].enabled) || (d==b))
                              {
                                  serialize_player_data(d, &a, snd->data);
                              }
                              message_board.add_message((char*)"Gamedata sent");
                              break;
        case IPX_NEWPLAYERDATA:snd->data[a++] = (char)type;
                              // b = index of new player
                              b = va_arg(ap, int);
                              serialize_player_data(b, &a, snd->data);
                              break;
        case IPX_IMJOININGNOW:snd->data[a++] = ( char ) type;
                              str = va_arg( ap, char *);
                              for (b=0;b<10;b++)
                               snd->data[a++]=*(str++);
                              break;
        case IPX_SERVERSEARCH:snd->data[a++] = ( char ) type;
                              break;
        case IPX_SERVERANSWER:snd->data[a++] = ( char ) type;
                              str = va_arg( ap, char * );
                              for ( b = 0; b < 10; b++ )
                              snd->data[a++ ] = str[b];
                              break;
        case IPX_SERVERSHUTDOWN:snd->data[a++] = ( char ) type;
                                break;
        case IPX_JOINREQUEST:snd->data[a++] = ( char ) type;
                             break;
        case IPX_SENDLEVELINFO:snd->data[a++] = ( char ) type;
                               break;
        case IPX_SENDLEVEL:snd->data[a++] = ( char ) type;
                           b = va_arg( ap, int);
                           snd->data[a++] = (char)(b & 255);
                           snd->data[a++] = (char)(b >> 8 );
                           break;
        case IPX_TRANSMITLEVEL:
                           snd->data[a++] = ( char ) type;
                           b = va_arg( ap, int);
                           snd->data[a++] = (char)(b & 255);
                           snd->data[a++] = (char)(b >> 8 );
                           str=(char*)level;
                           for (c=b*(MAXDATASIZE-24);(c<(b+1)*(MAXDATASIZE-24))&&(c<(12*level_x_size*level_y_size));c++)
                            snd->data[a++] = str[c];

                           break;
        case IPX_JOINACCEPT:snd->data[a++] = ( char ) type;
                            b = va_arg(ap, int);
                            snd->data[a++] = (char) b;
                            if (b) message_board.add_message((char*)"Accepting player...");
                              else message_board.add_message((char*)"Player NOT accepted...");

                            break;
        case IPX_LEVELINFO: snd->data[a++]=(char)type;
                            snd->data[a++]=(char)(level_x_size&255);
                            snd->data[a++]=(char)(level_x_size>>8);
                            snd->data[a++]=(char)(level_y_size&255);
                            snd->data[a++]=(char)(level_y_size>>8);
                            p=(char *)&general_level_info;
                            for (b=0;b<(int)sizeof(General_level_info);b++)
                             snd->data[a++]=p[b];
                            break;
        default:error("sendipxnow: message %d not yet implemented\n", type );
    }
    snd->data[a++] = IPX_END;
    tk_port::net::sendpacket( *node, snd, (word)(a+1) );
    va_end( ap );
}

void sendipx( int dest, int type, ... )
{   int b,a;
    unsigned int c;
    char *str;
    char d;

    float x,y;
//  char kakka[102];

    va_list ap;
    va_start( ap, type );

    switch( type )
    {
        case IPX_SOUND:
        ipxstream[dest][ipxofs[dest]++] = (char)type;
        d=va_arg(ap,int);
        ipxstream[dest][ipxofs[dest] ++ ] = d;
        d=va_arg(ap,int);
        ipxstream[dest][ipxofs[dest] ++ ] = d;
        b= va_arg(ap,int);
        str = (char *) &b;
        for (c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
        b= va_arg(ap,int);
        str = (char *) &b;
        for (c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
        b= va_arg(ap,int);
        str = (char *) &b;
        for (c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
        break;

        case IPX_NEWBULLET:
        ipxstream[dest][ipxofs[dest]++] = (char)type;
        d=va_arg(ap,int);
        ipxstream[dest][ipxofs[dest] ++ ] = d;
        b= va_arg(ap,int);
        str = (char *) &b;
        for (c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
        b = va_arg(ap,int);
        x = bullet[b].x;
        y = bullet[b].y;
        str = (char *) &x;
        for (c=0;c<sizeof(float);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
        str = (char *) &y;
        for (c=0;c<sizeof(float);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
        str = (char *) &bullet[b].angle;
        for (c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
        str = (char *) &bullet[b].time;
        for (c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
        break;

        case IPX_UPDCRATE:
        ipxstream[dest][ipxofs[dest]++] = (char)type;
        d= va_arg(ap,int);
        ipxstream[dest][ipxofs[dest] ++ ] = d;
        a = va_arg(ap,int);

        str = (char *) &a;
        for (c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
        str = (char *) &crate[a].type;
        for (c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
        str = (char *) &crate[a].weapon;
        for (c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
        str = (char *) &crate[a].bullet;
        for (c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
        str = (char *) &crate[a].energy;
        for (c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
        str = (char *) &crate[a].x;
        for (c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
        str = (char *) &crate[a].y;
        for (c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
        str = (char *) &crate[a].IN_USE;
        for (c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
        str = (char *) &crate[a].text_count;
        for (c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
        break;

        case IPX_ALIVE:
        ipxstream[dest][ipxofs[dest] ++ ] = (char)type;
        ipxstream[dest][ipxofs[dest] ++ ] = (char)aplayer[0]->tindex;
        break;

        case IPX_MESSAGE:
        ipxstream[dest][ipxofs[dest] ++ ] = (char)type;
        str=va_arg(ap,char *);
        for (c=0;c<MAX_MESSAGE_LENGTH;c++)
         ipxstream[dest][ipxofs[dest] ++ ] = *(str++);
        break;

        case IPX_BURNING:
        ipxstream[dest][ipxofs[dest]++] = (char) type;
        d=va_arg(ap,int);
        c=va_arg(ap,int);
        ipxstream[dest][ipxofs[dest]++] = (char) d;
        ipxstream[dest][ipxofs[dest]++] = (char) (c!=0? 1:0);
        break;

        case IPX_UPDKILLS:
        ipxstream[dest][ipxofs[dest]++] = (char)type;
        d=va_arg(ap, int);ipxstream[dest][ipxofs[dest]++] = d;
        d=va_arg(ap, int);ipxstream[dest][ipxofs[dest]++] = d;
        str = (char *) &player[(int)d].player_kills;
        for (c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
        break;

        case IPX_SENDCRATES:
        ipxstream[dest][ipxofs[dest] ++ ] = (char)type;
        c= va_arg(ap,int);
        ipxstream[dest][ipxofs[dest] ++ ] = c;
        break;

        case IPX_PLYXY:
        ipxstream[dest][ipxofs[dest]++] = (char)type;
        d=va_arg( ap,int );
        ipxstream[dest][ipxofs[dest]++] = d;
        x = player[(int)d].x;
        y = player[(int)d].y;

        str = (char *)&x;
        for (c=0;c<sizeof(float);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
        str = (char *)&y;
        for (c=0;c<sizeof(float);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
        break;

        case IPX_PLYRANGLE:
        ipxstream[dest][ipxofs[dest] ++ ] = (char)type;
        d=va_arg( ap,int );
        ipxstream[dest][ipxofs[dest] ++ ] = d;
        str = (char *)&player[(int)d].rangle;
        for (c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
        break;

        case IPX_PLYANIM:
        ipxstream[dest][ipxofs[dest] ++ ] = (char)type;
        d=va_arg( ap,int );
        ipxstream[dest][ipxofs[dest] ++ ] = d;
        str = (char *)&player[(int)d].ANIM;
        for (c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
        break;

        case IPX_PLAYERDISCONNECTED:
        ipxstream[dest][ipxofs[dest] ++ ] = (char)type;
        // d = player index
        d=va_arg( ap,int );
        ipxstream[dest][ipxofs[dest] ++ ] = d;
        break;

        default:error( "sendipx: Command not implemented yet!\n" );
    }
    docheckflush();
    va_end( ap );
}

bool filter_address_char( char c )
{
    return ( '0' <= c && c <= '9' ) || ('a' <= c && c <= 'z') || c == '.';
}

static const char* prompt_server_address()
{
    // Allocate space for 123.123.123.123 at max. Static so
    // that the address is remembered over gaming session.
    static char server_address[] = "127.0.0.1\0\0\0\0\0\0";
    const char title[] = "enter server address";
    int x;
    fadeout(virbuff, pal);
    load_efp("EFPS/COOL.EFP", picture, 0);
    FONT_NUM = 0;
    x = 160 - (str_length(title) / 2);
    draw_box1(x - 10, 75, 320 - x + 10, 125, 5);
    memcpy(virbuff, picture, 64000);
    writefonts2(x, 85, title, 1);
    fadein(virbuff, pal);
    i::clear_stack();
    readline(x, 100, sizeof(server_address), server_address, screen, filter_address_char);
    return server_address;
}

void setup_ipx()
{
    // TODO: Port could be configurable.
    const unsigned int port = 8099;
    int a;
    for ( a = 0; a < MAX_PLAYERS; a ++  ) ipxofs[a] = 0;
    if ( NETWORK_MODE == SERVER )
    {
        strcpy( servername, name1 );
        serveraddr.node = 0;
        if ( tk_port::net::create_server(port) )
        {
            error("opensocket: Couldn't open socket (%u)\n", port);
        }
    }
    else if (NETWORK_MODE == CLIENT)
    {
        // Connection to nonexisting server has long timeout within
        // SDL net for which time it blocks. It's not great UX but
        // will do for now.
        if ( tk_port::net::connect_server(prompt_server_address(), port) )
        {
            warn("opensocket: Couldn't open socket (%u)\n", port);
        }
    }
}

void desetup_ipx()
{
    struct nodeaddr n;

    n=tk_port::net::BROADCAST();
    flushipx();
    if ((GAME_MODE==NETWORK)&&(NETWORK_MODE==SERVER))
        sendipxnow(&n,IPX_SERVERSHUTDOWN);
    // Wait a while before closing sockets so that
    // clients have time to react to SERVERSHUTDOWN
    tk_port::sleep(50);
    tk_port::net::close_connections();
}

void server_add_player(char *name,struct nodeaddr *n)
{
    int colors[]={3*16,9*16,10*16,5*16,1*16,2*16,4*16,6*16};
    int a,b;

    for (a=0;a<MAX_PLAYERS && player[a].enabled;a++);
    strcpy(player[a].name,name);
    memcpy(&player[a].node,n,sizeof(struct nodeaddr));
    player[a].DEAD=0;
    player[a].enabled=0;
    memset(player[a].kills,0,DIFF_ENEMIES*4);
    player[a].total_kills=0;
    player[a].shooted=0;
    player[a].hitten=0;
    player[a].player_kills=0;
    player[a].color=colors[a];
    player[a].death_match_start_up();
    player[a].time_since_last_alive=0;

    sendipxnow(n,IPX_GAMEDATA,(int)a);
    for (b=1;b<MAX_PLAYERS;b++)
     if (b!=a)
      if (player[b].enabled)
       sendipxnow(&player[b].node,IPX_NEWPLAYERDATA,a);
}

void do_server_shit()
{
    int a,b=0,d, data, ofs;
    unsigned int c;
    int param1,param2,param3,param4;
    struct nodeaddr n;
    char buf[256],buf2[256];
    int pcom[256];
    int pcoms=0;
    float x,y;
    char *str;
    int pl;

    tk_port::net::accept_connections();

    for ( a = 0; a < RECEIVERS; a ++  )
    if ( tk_port::net::receive(rec[a]) )
    {
        data =  - 1;
        ofs = 0;
        while( data!= IPX_END )
        {
            data = rec[a]->data[ofs++];
           switch ( data )
            {
                case IPX_ALIVE:
                pcom[pcoms++]=data;
                pl=rec[a]->data[ofs++];
                player[pl].enabled=1;
                player[pl].time_since_last_alive=0;
                break;

                case IPX_MESSAGE:
                pcom[pcoms++]=data;
                for(c=0;c<MAX_MESSAGE_LENGTH;c++)
                 buf[c]=rec[a]->data[ofs++];
                message_board.add_message(buf);
                break;

                case IPX_BURNING:
                pcom[pcoms++]=data;
                pl=rec[a]->data[ofs++];
                player[pl].burning=rec[a]->data[ofs++];
                for (c=1;c<MAX_PLAYERS;c++)
                 if ((int)c!=pl)
                  if (player[c].enabled)
                   sendipx(c,IPX_BURNING,(char)pl,(char)player[pl].burning);
                break;

                case IPX_SOUND:
                pcom[pcoms++]=data;
                pl=rec[a]->data[ofs++]; //tindex
                param1=rec[a]->data[ofs++];
                str = (char *) &param2;
                for (c=0;c<sizeof(int);c++) *(str++)=rec[a]->data[ofs++];
                str = (char *) &param3;
                for (c=0;c<sizeof(int);c++) *(str++)=rec[a]->data[ofs++];
                str = (char *) &param4;
                for (c=0;c<sizeof(int);c++) *(str++)=rec[a]->data[ofs++];
                MIDASplaySample( samplep[param1], MIDAS_CHANNEL_AUTO, 0, param2, get_volume(aplayer[0]->x,aplayer[0]->y,param3,param4),get_pan(aplayer[0]->x,param3));
                for (c=1;c<MAX_PLAYERS;c++)
                 if ((int)c!=pl)
                  if (player[c].enabled)
                   sendipx(c,IPX_SOUND,(char)0,(char)param1,param2,param3,param4);
                break;

                case IPX_NEWBULLET:
                pcom[pcoms++]=data;
                pl=rec[a]->data[ofs++]; //tindex
                for (b=0;b<MAX_BULLETS && bullet[b].IN_USE;b++) ;
                bullet[b].count=0;
                bullet[b].IN_USE=1;
                bullet[b].P=&player[pl];
                bullet[b].index=pl;
                str = (char *) &d;
                for (c=0;c<sizeof(int);c++) *(str++)=rec[a]->data[ofs++];
                bullet[b].type=&bullet_type[d];
                str = (char *) &bullet[b].x;
                for (c=0;c<sizeof(float);c++) *(str++)=rec[a]->data[ofs++];
                str = (char *) &bullet[b].y;
                for (c=0;c<sizeof(float);c++) *(str++)=rec[a]->data[ofs++];
                str = (char *) &bullet[b].angle;
                for (c=0;c<sizeof(int);c++) *(str++)=rec[a]->data[ofs++];
                str = (char *) &bullet[b].time;
                for (c=0;c<sizeof(int);c++) *(str++)=rec[a]->data[ofs++];

                for (c=1;c<MAX_PLAYERS;c++)
                 if ((int)c!=pl)
                  if (player[c].enabled)
                   sendipx(c,IPX_NEWBULLET,(char)0,(int)d,(int)b);
                break;

                case IPX_UPDCRATE:
                pcom[pcoms++]=data;

                b=rec[a]->data[ofs++];
                str = (char *) &pl;
                for (c=0;c<sizeof(int);c++) *(str++)=rec[a]->data[ofs++];
                str = (char *) &crate[pl].type;
                for (c=0;c<sizeof(int);c++) *(str++)=rec[a]->data[ofs++];
                str = (char *) &crate[pl].weapon;
                for (c=0;c<sizeof(int);c++) *(str++)=rec[a]->data[ofs++];
                str = (char *) &crate[pl].bullet;
                for (c=0;c<sizeof(int);c++) *(str++)=rec[a]->data[ofs++];
                str = (char *) &crate[pl].energy;
                for (c=0;c<sizeof(int);c++) *(str++)=rec[a]->data[ofs++];
                str = (char *) &crate[pl].x;
                for (c=0;c<sizeof(int);c++) *(str++)=rec[a]->data[ofs++];
                str = (char *) &crate[pl].y;
                for (c=0;c<sizeof(int);c++) *(str++)=rec[a]->data[ofs++];
                str = (char *) &crate[pl].IN_USE;
                for (c=0;c<sizeof(int);c++) *(str++)=rec[a]->data[ofs++];
                str = (char *) &crate[pl].text_count;
                for (c=0;c<sizeof(int);c++) *(str++)=rec[a]->data[ofs++];
                crate[pl].text_angle=0;
                if (crate[pl].text_count!=0)
                        {
                            crate[pl].IN_USE=0;
                            crate[pl].wake_up_count=1;
                        }
                for (c=1;c<MAX_PLAYERS;c++)
                 if ((int)c!=b)
                  if (player[c].enabled)
                   sendipx(c,IPX_UPDCRATE,0,pl);
                break;

                case IPX_SENDCRATES:
                pcom[pcoms++]=data;
                pl=rec[a]->data[ofs++];

                for (c=0;c<MAX_CRATES;c++)
                 if (crate[c].IN_USE)
                   sendipx(pl,IPX_UPDCRATE,0,c);
                break;

                case IPX_PLYXY:
                pcom[pcoms++]=data;
                pl=rec[a]->data[ofs++];
                str = (char *)&x;
                for (c=0;c<sizeof(float);c++) str[c]=rec[a]->data[ofs++];
                str = (char *)&y;
                for (c=0;c<sizeof(float);c++) str[c]=rec[a]->data[ofs++];
                player[pl].x=x;
                player[pl].y=y;
                for (c=1;c<MAX_PLAYERS;c++)
                 if ((int)c!=pl)
                  if (player[c].enabled)
                   sendipx(c,IPX_PLYXY,(char)pl);
                break;

                case IPX_UPDKILLS:
                pcom[pcoms++]=data;
                pl=rec[a]->data[ofs++]; //tindex
                b=rec[a]->data[ofs++];

                str = (char *) &player[b].player_kills;
                for (c=0;c<sizeof(int);c++) *(str++)=rec[a]->data[ofs++];
                for (c=1;c<MAX_PLAYERS;c++)
                 if ((int)c!=pl)
                  if (player[c].enabled)
                   sendipx(c,IPX_UPDKILLS,0,b);
                break;


                case IPX_PLYRANGLE:
                pcom[pcoms++]=data;
                pl=rec[a]->data[ofs++];
                str = (char *)&player[pl].rangle;
                for (c=0;c<sizeof(int);c++) str[c]=rec[a]->data[ofs++];
                for (c=1;c<MAX_PLAYERS;c++)
                 if ((int)c!=pl)
                  if (player[c].enabled)
                   sendipx(c,IPX_PLYRANGLE,pl);
                break;

                case IPX_PLYANIM:
                pcom[pcoms++]=data;
                pl=rec[a]->data[ofs++];
                str = (char *)&player[pl].ANIM;
                for (c=0;c<sizeof(int);c++) str[c]=rec[a]->data[ofs++];
                for (c=1;c<MAX_PLAYERS;c++)
                 if ((int)c!=pl)
                  if (player[c].enabled)
                   sendipx(c,IPX_PLYANIM,pl);
                break;

                case IPX_IMJOININGNOW:
                pcom[pcoms++]=data;
                n.node=rec[a]->header.src.node;
                for(b=0;b<10;b++)
                 buf[b]=rec[a]->data[ofs++];
                server_add_player(buf,&n);
                break;

                case IPX_SERVERSEARCH:
                pcom[pcoms++]=data;
                n.node=rec[a]->header.src.node;
                sendipxnow(&n, IPX_SERVERANSWER,servername);
                break;

                case IPX_TEXTMESSAGE:
                pcom[pcoms++]=data;
                ofs+=IPXMSGLEN[IPX_TEXTMESSAGE]-1;
                break;

                case IPX_END:
                pcom[pcoms++]=data;
                break;

                case IPX_JOINREQUEST:
                pcom[pcoms++]=data;
                n.node=rec[a]->header.src.node;
                sendipxnow(&n, IPX_JOINACCEPT, player_amount() < MAX_PLAYERS ? 1 : 0);
                break;

                case IPX_SENDLEVELINFO:
                pcom[pcoms++]=data;
                n.node=rec[a]->header.src.node;
                sendipxnow(&n, IPX_LEVELINFO);
                break;

                case IPX_SENDLEVEL:
                pcom[pcoms++]=data;
                n.node=rec[a]->header.src.node;
                b=rec[a]->data[ofs++];
                b+=rec[a]->data[ofs++]<<8;
                sendipxnow(&n, IPX_TRANSMITLEVEL, (int) b);
                break;

                default:
                sprintf(buf,"do_server_shit: receiver %d\nunknown server command %d at offset %d\n", a, data,ofs );
                sprintf(buf2,"Previous commands:\n");
                strcat(buf,buf2);
                for (b=0;b<pcoms;b++)
                 {
                    sprintf(buf2,"%d\n",pcom[b]);
                    strcat(buf,buf2);
                 }
                error(buf);
            };
        }

    }
    flushipx();
}

static void deserialize_player_data(int* data_index, char* buffer)
{
    char *str;
    unsigned int a;
    int pno = buffer[(*data_index)++];
    if (player[pno].enabled) error("realjoin: IPX_GAMEDATA mismatch detected!\n(player[%d].enabled==1)", pno);
    str = (char *)&player[pno].x;
    for (a = 0;a < sizeof(float);a++) str[a] = buffer[(*data_index)++];
    str = (char *)&player[pno].y;
    for (a = 0;a < sizeof(float);a++) str[a] = buffer[(*data_index)++];
    str = (char *)&player[pno].rangle;
    for (a = 0;a < sizeof(int);a++) str[a] = buffer[(*data_index)++];
    player[pno].color = buffer[(*data_index)++];
    str = (char *)&player[pno].burning;
    for (a = 0;a < sizeof(int);a++) str[a] = buffer[(*data_index)++];
    str = (char *)&player[pno].node;
    for (a = 0;a < sizeof(struct nodeaddr);a++) str[a] = buffer[(*data_index)++];
    for (a = 0;a < 10;a++) player[pno].name[a] = buffer[(*data_index)++];
    str = (char *)&player[pno].ANIM;
    for (a = 0;a < sizeof(int);a++) str[a] = buffer[(*data_index)++];
    str = (char *)&player[pno].player_kills;
    for (a = 0;a < sizeof(int);a++) str[a] = buffer[(*data_index)++];
    str = (char *)&player[pno].DEAD;
    for (a = 0;a < sizeof(int);a++) str[a] = buffer[(*data_index)++];
    player[pno].enabled = 1;
}

void do_client_shit()
{
    int a,b,d, data, ofs;
    unsigned int c;
    int param1,param2,param3,param4;
//  struct nodeaddr n;
    char buf[1024],buf2[1024];
    int pcom[256];
    int pcoms=0;
    float x,y;
    char *str;
    int pl;

    // Check that server connection is still alive
    if ( !tk_port::net::active_connections_exist() )
    {
        warn("Lost connection to server\n");
        quit = 1;
        return;
    }

    for ( a = 0; a < RECEIVERS; a ++  )
    if ( tk_port::net::receive(rec[a]) )
    {
        data =  - 1;
        ofs = 0;
        while( data!= IPX_END )
        {
            data = rec[a]->data[ofs++];
            switch ( data )
            {
                case IPX_SOUND:
                pcom[pcoms++]=data;
                pl=rec[a]->data[ofs++]; //tindex
                param1=rec[a]->data[ofs++];
                str = (char *) &param2;
                for (c=0;c<sizeof(int);c++) *(str++)=rec[a]->data[ofs++];
                str = (char *) &param3;
                for (c=0;c<sizeof(int);c++) *(str++)=rec[a]->data[ofs++];
                str = (char *) &param4;
                for (c=0;c<sizeof(int);c++) *(str++)=rec[a]->data[ofs++];
                MIDASplaySample( samplep[param1], MIDAS_CHANNEL_AUTO, 0, param2, get_volume(aplayer[0]->x,aplayer[0]->y,param3,param4),get_pan(aplayer[0]->x,param3));
                break;

                case IPX_NEWBULLET:
                pcom[pcoms++]=data;
                pl=rec[a]->data[ofs++]; //tindex
                for (b=0;b<MAX_BULLETS && bullet[b].IN_USE;b++) ;
                bullet[b].count=0;
                bullet[b].IN_USE=1;
                bullet[b].P=&player[pl];
                bullet[b].index=pl;
                str = (char *) &d;
                for (c=0;c<sizeof(int);c++) *(str++)=rec[a]->data[ofs++];
                bullet[b].type=&bullet_type[d];
                str = (char *) &bullet[b].x;
                for (c=0;c<sizeof(float);c++) *(str++)=rec[a]->data[ofs++];
                str = (char *) &bullet[b].y;
                for (c=0;c<sizeof(float);c++) *(str++)=rec[a]->data[ofs++];
                str = (char *) &bullet[b].angle;
                for (c=0;c<sizeof(int);c++) *(str++)=rec[a]->data[ofs++];
                str = (char *) &bullet[b].time;
                for (c=0;c<sizeof(int);c++) *(str++)=rec[a]->data[ofs++];
                break;

                case IPX_SERVERSHUTDOWN:
                quit=1;
                break;

                case IPX_MESSAGE:
                pcom[pcoms++]=data;
                for(c=0;c<MAX_MESSAGE_LENGTH;c++)
                 buf[c]=rec[a]->data[ofs++];
                message_board.add_message(buf);
                break;


                case IPX_BURNING:
                pcom[pcoms++]=data;
                pl=rec[a]->data[ofs++];
                player[pl].burning=rec[a]->data[ofs++];
                break;

                case IPX_UPDKILLS:
                pcom[pcoms++]=data;
                pl=rec[a]->data[ofs++]; //tindex
                b=rec[a]->data[ofs++];

                str = (char *) &player[b].player_kills;
                for (c=0;c<sizeof(int);c++) *(str++)=rec[a]->data[ofs++];
                break;

                case IPX_UPDCRATE:
                pcom[pcoms++]=data;

                b=rec[a]->data[ofs++];
                str = (char *) &pl;
                for (c=0;c<sizeof(int);c++) *(str++)=rec[a]->data[ofs++];
                str = (char *) &crate[pl].type;
                for (c=0;c<sizeof(int);c++) *(str++)=rec[a]->data[ofs++];
                str = (char *) &crate[pl].weapon;
                for (c=0;c<sizeof(int);c++) *(str++)=rec[a]->data[ofs++];
                str = (char *) &crate[pl].bullet;
                for (c=0;c<sizeof(int);c++) *(str++)=rec[a]->data[ofs++];
                str = (char *) &crate[pl].energy;
                for (c=0;c<sizeof(int);c++) *(str++)=rec[a]->data[ofs++];
                str = (char *) &crate[pl].x;
                for (c=0;c<sizeof(int);c++) *(str++)=rec[a]->data[ofs++];
                str = (char *) &crate[pl].y;
                for (c=0;c<sizeof(int);c++) *(str++)=rec[a]->data[ofs++];
                str = (char *) &crate[pl].IN_USE;
                for (c=0;c<sizeof(int);c++) *(str++)=rec[a]->data[ofs++];
                str = (char *) &crate[pl].text_count;
                for (c=0;c<sizeof(int);c++) *(str++)=rec[a]->data[ofs++];
                crate[pl].text_angle=0;
                if (crate[pl].text_count!=0) crate[pl].IN_USE=0; else
                    new_effect( &normal_l, TELEPORT, crate[pl].x + 8, crate[pl].y + 8, 0, 0 );
                break;

                case IPX_PLYXY:
                pcom[pcoms++]=data;
                pl=rec[a]->data[ofs++];
                str = (char *)&x;
                for (c=0;c<sizeof(float);c++) str[c]=rec[a]->data[ofs++];
                str = (char *)&y;
                for (c=0;c<sizeof(float);c++) str[c]=rec[a]->data[ofs++];
                player[pl].x=x;
                player[pl].y=y;
                break;

                case IPX_PLYRANGLE:
                pcom[pcoms++]=data;
                pl=rec[a]->data[ofs++];
                str = (char *)&player[pl].rangle;
                for (c=0;c<sizeof(int);c++) str[c]=rec[a]->data[ofs++];
                break;

                case IPX_PLYANIM:
                pcom[pcoms++]=data;
                pl=rec[a]->data[ofs++];
                str = (char *)&player[pl].ANIM;
                for (c=0;c<sizeof(int);c++) str[c]=rec[a]->data[ofs++];
                break;

                case IPX_END:
                pcom[pcoms++]=data;
                break;

                case IPX_NEWPLAYERDATA:
                pcom[pcoms++] = data;
                deserialize_player_data( &ofs, rec[a]->data );
                break;

                case IPX_PLAYERDISCONNECTED:
                pcom[pcoms++] = data;
                pl = rec[a]->data[ofs++];
                player[pl].enabled = false;
                break;

                default:
                sprintf(buf,"do_client_shit: receiver %d\nunknown client command %d at offset %d\n", a, data,ofs );
                sprintf(buf2,"Previous commands:\n");
                strcat(buf,buf2);
                for (b=0;b<pcoms;b++)
                 {
                    sprintf(buf2,"%d\n",pcom[b]);
                    strcat(buf,buf2);
                 }
                error(buf);
            };
        }
    }
    sendipx(0,IPX_ALIVE);
    flushipx();
}

int realjoin()
{   int received=0;
    int a,c;
    int plyers=0;
    int timeout,s;
    int data,ofs;

    for (a=0;a<MAX_PLAYERS;a++)
     player[a].enabled=0;

    while(!received)
    {
        sendipxnow(&serveraddr,IPX_IMJOININGNOW, name1);
        timeout=0;

        s=framecount;

        while((!received)&&(!timeout))
        {
            if (i::state(k::ESC)) return 0;
            for ( a = 0; a < RECEIVERS; a ++  )
            if ( tk_port::net::receive(rec[a]) )
            {
                data =  - 1;
                ofs = 0;
                while( data!= IPX_END )
                {
                    data = rec[a]->data[ofs++];
                    switch ( data )
                    {
                        case IPX_GAMEDATA:
                        plyers=rec[a]->data[ofs++];
                        aplayer[0]=&(player[(int)rec[a]->data[ofs++]]);
                        aplayer[0]->speed=rec[a]->data[ofs++];
                        for (c=0;c<plyers;c++)
                        {
                            deserialize_player_data( &ofs, rec[a]->data );
                        }
                        if (!aplayer[0]->enabled) message_board.add_message((char*)"realjoin: something missing...");
                        received=1;
                        if (rec[a]->data[ofs]!=IPX_END) error("realjoin: IPX packet mismatching!");

                        break;
                        default:ofs+=IPXMSGLEN[data]-1;
                    };
                }
            }
            if ((int)(framecount-s)>target_frames) timeout=1;
        }
    }
    return 1;
}

int getlevel()
{
    int received,timeout;
    int blocks,bno,c;
    int s,b,a,data,ofs;
    char *p;

    blocks=(12*level_x_size*level_y_size)/(MAXDATASIZE-24);
    if ((12*level_x_size*level_y_size)%(MAXDATASIZE-24)) blocks++;
    level=(struct BLOCK*)malloc(12*level_x_size*level_y_size);

    for (b=0;b<blocks;b++)
    {
    received=0;

    while(!received)
    {
        sendipxnow(&serveraddr,IPX_SENDLEVEL, b);
        timeout=0;

        s=framecount;

        while((!received)&&(!timeout))
        {
            if (i::state(k::ESC)) return 0;
            for ( a = 0; a < RECEIVERS; a ++  )
            if ( tk_port::net::receive(rec[a]) )
            {
                data =  - 1;
                ofs = 0;
                while( data!= IPX_END )
                {
                    data = rec[a]->data[ofs++];
                    switch ( data )
                    {
                        case IPX_TRANSMITLEVEL:
                        bno = rec[a]->data[ofs++];
                        bno+= rec[a]->data[ofs++]<<8;
                        if (bno!=b) return 0;
                        p   = (char*)level;
                        for (c=b*(MAXDATASIZE-24);(c<(b+1)*(MAXDATASIZE-24))&&(c<(12*level_x_size*level_y_size));c++)
                         p[c]=rec[a]->data[ofs++];
                        received=1;
                        break;
                        default:ofs+=IPXMSGLEN[data]-1;
                    };
                }
            }
            if ((int)(framecount-s)>target_frames) timeout=1;
        }
    }
    }
    return 1;
}

int getlevelinfo()
{   int received,timeout;
    int s,b,a,data,ofs;
    char *p;

    received=0;
    while(!received)
    {
        sendipxnow(&serveraddr,IPX_SENDLEVELINFO);
        timeout=0;

        s=framecount;

        while((!received)&&(!timeout))
        {
            if (i::state(k::ESC)) return 0;
            for ( a = 0; a < RECEIVERS; a ++  )
            if ( tk_port::net::receive(rec[a]) )
            {
                data =  - 1;
                ofs = 0;
                while( data!= IPX_END )
                {
                    data = rec[a]->data[ofs++];
                    switch ( data )
                    {
                        case IPX_LEVELINFO:
                        level_x_size =rec[a]->data[ofs++];
                        level_x_size+=rec[a]->data[ofs++]<<8;
                        level_y_size =rec[a]->data[ofs++];
                        level_y_size+=rec[a]->data[ofs++]<<8;
                        p=(char *)&general_level_info;
                        for (b=0;b<(int)sizeof(General_level_info);b++)
                         p[b]=rec[a]->data[ofs++];
                        received=1;
                        break;
                        default:ofs+=IPXMSGLEN[data]-1;
                    };
                }
            }
            if ((int)(framecount-s)>target_frames) timeout=1;
        }
    }
    return 1;
}

int join_game(struct nodeaddr n)
{   char buf[256];

    memcpy(&serveraddr,&n,sizeof(serveraddr));
    load_efp( "EFPS/COOL.EFP", picture, 0 );
    draw_box1( 60, 10, 320 - 60, 190, 15 );
    fadeout( virbuff, pal );
    memcpy(virbuff,picture,64000);
    FONT_NUM = 0;
    writefonts2( 160 - ( str_length( "joining game" )  >> 1 ), 20, "joining game", 1 );
    fadein(virbuff,pal);
    FONT_NUM = 1;
    writefonts( 80, 50, "receiving levelinfo...", M_BLUE );
    memcpy(screen,virbuff,64000);
    if (!getlevelinfo()) return 0;
    sprintf(buf,"  \"%s\" (%d x %d)...",general_level_info.comment,level_x_size,level_y_size);
    writefonts( 80, 60,buf, M_BLUE );
    writefonts( 80, 70,"receiving level...", M_BLUE );
    memcpy(screen,virbuff,64000);
    if (!getlevel()) return 0;
    writefonts( 80, 80,"joining game...", M_BLUE );
    memcpy(screen,virbuff,64000);
    if (!realjoin()) return 0;
    sprintf(buf,"%d players",player_amount());
    writefonts( 80, 90,buf, M_BLUE );
    writefonts( 80, 100,"done!", M_BLUE );
    memcpy(screen,virbuff,64000);

    return 1;
}

int waitfor(int msg,int secs)
{   int timeout=0,s;
    int a,data,ofs,yes=0;

    s=framecount;

    while ((!timeout)&&(yes==0))
    {
        for ( a = 0; a < RECEIVERS; a ++  )
        if ( tk_port::net::receive(rec[a]) )
        {
            data =  - 1;
            ofs = 0;
            while( data!= IPX_END )
            {
                data = rec[a]-> data[ofs ++ ];
                if (data==msg)
                {
                    if (rec[a]-> data[ofs ++ ]) yes=1;
                                           else yes=-1;
                } else ofs+=IPXMSGLEN[data]-1;
            }
        }
        if ((int)(framecount-s)>target_frames*secs) timeout=1;
    }
    if ((yes<0)||(timeout)) return 0;
                       else return 1;
}

int join_server(struct nodeaddr n)
{
    memcpy(&serveraddr,&n,sizeof(serveraddr));
    FONT_NUM = 1;
    writefonts(80,190,"waiting...",M_BLUE);
    memcpy(screen,virbuff,64000);
    sendipxnow(&serveraddr,IPX_JOINREQUEST);
    if (!waitfor(IPX_JOINACCEPT,3)) return 0;
    return 1;
}

int select_server()
{
    int a, b, c,data,ofs,first=1,done=0,cnt=0,oclock=0;
    int select=0;
    int chg=1;
    int servers=0;
    char servernames[10][10],tempname[10];
    struct nodeaddr nodez[10],tempnode;
    struct nodeaddr bc;

    load_efp( "EFPS/COOL.EFP", picture, 0 );
    draw_box1( 60, 10, 320 - 60, 190, 15 );
    fadeout( virbuff, pal );

    i::clear(k::ESC);
    i::clear(k::ENTER);
    bc=tk_port::net::BROADCAST();

    while ( !done )
    {
        if (i::state(k::ENTER) && servers>0)
        {
            if (join_server(nodez[select])) done=1;
            else
            {
                chg=1;
                servers--;
                for (c=select;c<servers;c++)
                {
                    memcpy(&nodez[c],&nodez[c+1],sizeof(nodeaddr));
                    strcpy(servernames[c],servernames[c+1]);
                }
            }
            i::clear(k::ENTER);
        }
        if (servers>0){
         if (i::state(k::ARROW_UP) && select>0) {select--;i::clear(k::ARROW_UP);}
         if (i::state(k::ARROW_DOWN) && select<servers-1) {select++;i::clear(k::ARROW_DOWN);}
        }
        memcpy( virbuff, picture, 64000 );
        FONT_NUM = 0;
        if (chg) writefonts2( 160 - ( str_length( "select server" )  >> 1 ), 20, "select server", 1 );
        FONT_NUM = 1;
        if (chg)
        {
            for (a=0;a<servers;a++)
            {
                if (select!=a)
                    writefonts( 100,50+a*10,servernames[a],M_BLUE );
                else
                    writefonts( 100,50+a*10,servernames[a],M_YELLOW );
            }
        }
        if (!first) {
                        if (chg) memcpy( screen, virbuff, 64000);
                        chg=0;
                    }
                else
                {
                    fadein(virbuff,pal);
                    first=0;
                }
        tk_port::event_tick();
        if ( clock() != oclock )
        {
            oclock = clock();
           if (servers==0)
            draw_ball( 62,18, cnt );
             else draw_ball(70,45+select*10,cnt);
            cnt ++;
            if ( cnt > 23 ) cnt = 0;

            sendipxnow(&bc, IPX_SERVERSEARCH);
        }

        for ( a = 0; a < RECEIVERS; a ++  )
        if ( tk_port::net::receive(rec[a]) )
        {
            data =  - 1;
            ofs = 0;
            while( data!= IPX_END )
            {
                data = rec[a]-> data[ofs ++ ];
                switch ( data )
                {
                    case IPX_SERVERANSWER:
                    tempnode.node=rec[a]->header.src.node;
                    for (b=0;b<10;b++)
                     tempname[b]=rec[a]->data[ofs++];
                    if (servers<10)
                    {
                        for (b=0;(b<servers)&&(memcmp(&tempnode,&nodez[b],sizeof(nodeaddr))!=0);b++);
                        if (b==servers)
                        {
                            chg=1;
                            memcpy(&nodez[servers],&tempnode,sizeof(nodeaddr));
                            strcpy(servernames[servers],tempname);
                            servers++;
                        }
                    }
                    break;

                    case IPX_SERVERSHUTDOWN:
                    tempnode.node=rec[a]->header.src.node;
                    for (b=0;b<servers&&memcmp(&tempnode,&nodez[b],sizeof(nodeaddr))!=0;b++);
                    if (b!=servers)
                    {
                        chg=1;
                        servers--;
                        for (c=b;c<servers;c++)
                         {
                            memcpy(&nodez[c],&nodez[c+1],sizeof(nodeaddr));
                            strcpy(servernames[c],servernames[c+1]);
                         }
                    }
                    break;

                    case IPX_TEXTMESSAGE:
                    while ( ( data = rec[a]-> data[ofs ++ ] ) != 0 );
                    break;

                    case IPX_END:
                    break;
                    default:ofs+=IPXMSGLEN[data]-1;
                };
            }
        }
        if (i::state(k::ESC)) return 0;
    }

    if (join_game(nodez[select])) return 1;
                             else return 0;
}

void chk_for_loosers()
{
    int a,b;
    char message[100];

    for (a=1;a<MAX_PLAYERS;a++)
     if (player[a].enabled)
      if (player[a].time_since_last_alive++>=40*3)
       {
        player[a].enabled=0;
        sprintf(message,"%s (%d) left the game",player[a].name,player[a].tindex);
        for (b=1;b<MAX_PLAYERS;b++)
         if (player[b].enabled)
         {
          sendipx(b, IPX_MESSAGE, message);
          sendipx(b, IPX_PLAYERDISCONNECTED, (char) a);
         }
        message_board.add_message(message);
       }
}

void game()
{
    int c, a;
    int frame_count;
    int oclock;
    int op[5], opcnt = 0, opcnt2 = 0;
    char match_level[50];
    char filename[50];
    char text[10];

    for (a=0;a<MAX_PLAYERS;a++)
       player[a].tindex=a;

    if ( !(GAME_MODE==NETWORK && NETWORK_MODE==CLIENT))
     for (a=0;a<MAX_PLAYERS;a++)     player[a].enabled=0;

    if ( GAME_MODE == SPLIT_SCREEN &&KILLING_MODE!= DEATHMATCH ) episode = DEATH_MATCH_EPISODE - 1;
    if ( GAME_MODE == ONE_PLAYER ) {aplayer[0]=&(player[0]);}
    if ( GAME_MODE == SPLIT_SCREEN )
    {
        ACTIVE_PLAYERS = 2;
        aplayer[0]=&player[0];
        aplayer[1]=&player[1];
    }
    else { ACTIVE_PLAYERS = 1;}

    if (GAME_MODE==NETWORK && NETWORK_MODE==SERVER)
    {
     player[0].enabled=1;
     aplayer[0]=&(player[0]);

     aplayer[0]->node.node=0;
    }

    if ((GAME_MODE!=NETWORK)||
        (NETWORK_MODE==SERVER))
    {
        strcpy( match_level, "LEVS/" );
        if ( DEATH_MATCH_EPISODE > 0 )
        {
            strcat( match_level, dirlist[DEATH_MATCH_EPISODE - 1].name );
            strcat( match_level, "/" );
        }
        strcat( match_level, DEATH_MATCH_LEVEL );
        LEVELS = files_in_episode[episode + 1];
        LEVEL = START_LEVEL;
        if ( LEVEL > LEVELS - 1 ) LEVEL = LEVELS - 1;
    }

    fadeout( virbuff, pal );
    set_keys();
    if ( GAME_MODE == NETWORK && NETWORK_MODE == CLIENT )
    {
        alusta_client();
        sendipx( 0, IPX_ALIVE );
        sendipx( 0, IPX_SENDCRATES, (char) aplayer[0]->tindex );
    }
    else
    {
        alusta_players1();
    }
    quit = 0;
    all_dead = 0;
    BACK_TO_MENU = 0;
    while ( !quit &&!BACK_TO_MENU )
    {
        //  //
        if ((GAME_MODE!=NETWORK)||
            (NETWORK_MODE==SERVER))
        {
            strcpy( filename, "LEVS/" );
            strcat( filename, dirlist[episode].name );
            strcat( filename, "/LEVEL" );
            sprintf( text, "%d", LEVEL + 1 );
            strcat( filename, text );
            strcat( filename, ".LEV" );
            if ( KILLING_MODE != DEATHMATCH ) {
                leveldata.load( filename );
            }
            else {
                leveldata.load( match_level );
            }
            if ( KILLING_MODE != DEATHMATCH )
                level_info();
        }
        if ( !BACK_TO_MENU )
        {
            if ( DARK_MODE )
            {
                for ( a = 0; a < ACTIVE_PLAYERS; a ++  )
                aplayer[a]->see_level = (char *)malloc( level_x_size*level_y_size );
            }
            if ( KILLING_MODE!= DEATHMATCH )
            {
                do_prices();
                if ( LEVEL > 0 )
                 for ( a = 0; a < ACTIVE_PLAYERS; a ++  ) aplayer[a]->shop();
            }

            tk_port::change_resolution( 240 );

            peli_biisi();

            load_efp_pal( "EFPS/WALLS1.EFP", pal );


            if ( ! (GAME_MODE == NETWORK && NETWORK_MODE == CLIENT))
                alusta_players2();
            DEAD_ENEMIES = 0;


            if ((GAME_MODE != NETWORK) || (NETWORK_MODE == SERVER))
            {
                if ( ENEMIES_ON_GAME == 0 && KILLING_MODE == DEATHMATCH )
                {
                    ENEMIES = 0;
                }
                else
                {
                    alusta_enemies();
                    place_enemies();
                }
            }
            else
            {
                ENEMIES = 0;
            }

            null_body_parts();
            null_effects();
            null_bullets();
            null_crates();
            if ((GAME_MODE != NETWORK) || (NETWORK_MODE == SERVER))
            {
                if ( KILLING_MODE == COOPERATIVE ) place_normal_game_crates();
                else place_death_match_crates();
            }
            FONT_NUM = 1;
            frame_count = 0;
            oclock = clock();
            st = clock();
            first = 1;
            OVER_POWER = 0;
            Spot_count = 0;
            Steam_count = 0;
            next_level = 0;

            if ((GAME_MODE!=NETWORK)||
                (NETWORK_MODE==SERVER))
                  if ( KILLING_MODE == DEATHMATCH )
                    for ( a = 0; a < ACTIVE_PLAYERS; a++ ) aplayer[a]->random_place();

            effect_age_count = 0;
            message_board.clear();

            while( !all_dead &&!quit &&next_level < C_DELAY )
            {
                // GAME loop
                c = clock();
                if ( ( ( float ) c / CLOCKS_PER_SEC ) - ( ( float ) oclock / CLOCKS_PER_SEC ) > 2 )
                {
                    F_PER_SEC = ( ( float ) frame_count / ( c - oclock )  ) *CLOCKS_PER_SEC;
                    oclock = c;
                    frame_count = 0;
                }
                if (FRAMES_ON) opcnt ++; // OVER_POWER count

                if ( framecount )
                {
                   if (FRAMES_ON){
                    const int op_table_size = sizeof( op ) / sizeof( op[0] );
                    op[opcnt2] = opcnt;
                    opcnt2 ++;
                    if ( opcnt2 >= op_table_size) opcnt2 = 0;
                    opcnt = 0;
                    if ( opcnt2 == 0 )
                    OVER_POWER = ( op[0] + op[1] + op[2] + op[3] + op[4] ) / op_table_size;
                    }
                    framecount = 0;// incremented by MIDAS timer
                    game_shit();// do the game !!!!

                    if ( GAME_MODE == NETWORK )
                    {
                        switch ( NETWORK_MODE )
                        {
                        case CLIENT:
                            do_client_shit();
                            break;
                        case SERVER:
                            chk_for_loosers();
                            do_server_shit();
                            break;
                        default:
                            error( "Invalid network mode" );
                        }
                    }

                    Steam_count ++;
                    Steam_count%= 360;
                    Spot_count += 2;
                    Spot_count%= 360;
                    frame_count ++;
                }
                tk_port::event_tick();

                if ( tk_port::quit_flag )
                {
                    quit = 1;
                    BACK_TO_MENU = 1;
                }
            }  // game loop end

            if ( DARK_MODE )
                for ( a = 0; a < ACTIVE_PLAYERS; a ++  )
                    free( aplayer[a]->see_level );
        }
        fadeout( virbuff, pal );
        menu_biisi();

        tk_port::change_resolution( 200 );

        if ( KILLING_MODE!= DEATHMATCH )
        {
            all_dead = 0;
            are_all_dead();
            if ( all_dead ) quit = 1;
                       else next_level = 1;
            if ( LEVEL == 0 &&BACK_TO_MENU )
            {   }
            else statics();

            if ( !all_dead && GAME_MODE == SPLIT_SCREEN ) two_to_one();

            if ( LEVEL == LEVELS - 1 )
             if ( !BACK_TO_MENU )
              if ( next_level == 1 )
               if ( quit == 0 )
                {
                    quit = 1; next_level = 0; victory();
                }
            if ( next_level ) LEVEL ++;
            next_level = 0;
        }
    }
    load_efp_pal( "EFPS/WARE.EFP", pal );
}

void menu_scr()
{
    load_efp( "EFPS/TK.EFP", picture, 0 );
    load_efp_pal( "EFPS/WARE.EFP", pal );
    draw_box1( 55, 90 - ( ( Menu_texts*15 )  / 2 ), 320 - 55, 110 + ( ( Menu_texts*15 )  / 2 ), 10 );
    i::clear(k::ENTER);
    first = 1;
}

void menu()
{
    int a, cnt = 0, selected = 0, quit = 0, oclock;
    int starty = ( Menu_texts*15 )  / 2;
    ph = 0;
    menu_scr();
    menu_biisi();
    while( !quit )
    {
        memcpy( virbuff, picture, 64000 );
        FONT_NUM = 1;
        writefonts( 320 - str_length( Version ) - 2, 190, Version, M_BLUE );
        for ( a = 0; a < Menu_texts; a ++  )
        {
            if ( a == selected )
            {
                FONT_NUM = 0;
                writefonts2( 160 - ( str_length( menu_text[a] )  / 2 ), 100 - starty + ( a*15 ), menu_text[a], 1 );
            }
            else
            {
                FONT_NUM = 2;
                writefonts2( 160 - ( str_length( menu_text[a] )  / 2 ), 100 - starty + ( a*15 ), menu_text[a], M_RED );
            }
        }
        if ( first == 1 )
        {
            fadein( virbuff, pal ); first = 0;
        }
        memcpy( screen, virbuff, 64000 );
        tk_port::event_tick();
        oclock = clock();
        while (!(quit || i::state(k::ARROW_UP) || i::state(k::ARROW_DOWN) || i::state(k::ENTER)))
        {
            if ( clock() != oclock )
            {
                oclock = clock();
                draw_ball( 55, 100 - starty + ( selected*15 )  - 2, cnt );
                draw_ball( 320 - 75, 100 - starty + ( selected*15 )  - 2, cnt );
                cnt ++;
                if ( cnt > 23 ) cnt = 0;
            }
            tk_port::event_tick();
            if (tk_port::quit_flag) {
                quit = 1;
            }
        };
        MIDASplaySample( samplep[KLIKWAV], MIDAS_CHANNEL_AUTO, 0, 22500, EFFECT_VOLUME, MIDAS_PAN_MIDDLE );
        if ( i::state(k::ARROW_UP) )
        {
            selected --;if ( selected < 0 ) selected = Menu_texts - 1; i::clear(k::ARROW_UP);
        }
        if ( i::state(k::ARROW_DOWN) )
        {
            selected ++;if ( selected > Menu_texts - 1 ) selected = 0; i::clear(k::ARROW_DOWN);
        }
        if ( i::state(k::ENTER) )
        {
            if ( selected == 0 )
            {
                GAME_MODE = ONE_PLAYER;
                KILLING_MODE = COOPERATIVE;
                episode = choose_episode();
                if ( episode != INVALID_EPISODE )
                {
                    game();
                }
                menu_scr();
            }
            if ( selected == 1 )
            {
                multiplayer_options(); menu_scr();
            }
            if ( selected == 2 )
            {
                options(); menu_scr();
            }
            if ( selected == 3 ) quit = 1;
        }
    }
    fadeout( virbuff, pal );
}

void credits()
{
    std::cout << "\n";
    std::cout << "              Thanks for playing --The Ultimate TK-- (Tapan Kaikki) !  \n";
    std::cout << "\n";
    std::cout << "  Credits: \n";
    std::cout << "\n";
    std::cout << "  Coding: Allu & Unski \n";
    std::cout << "  Graphics: Allu \n";
    std::cout << "  Music: Raipe, Allu & Unski \n";
    std::cout << "  Sound effects: Allu (let's rip) \n";
    std::cout << "  Episodes: Blanket & Allu \n";
    std::cout << "  Thanks to - MIDAS sound system \n";
    std::cout << "            - Vertigo for svga routines\n";
    std::cout << "\n";
    std::cout << "  Also thanks to Janne Laitinen (game name), Niko Kotilainen & \n";
    std::cout << "  all TK fans in the world ! \n";
    std::cout << "\n";
    std::cout << "  Contact the Error Free Productions by e-mail: \n";
    std::cout << "\n";
    std::cout << "   ef@tuusniemi.fi \n";
    std::cout << "\n";
    std::cout << "  And visit the Error Free Productions homepage: \n";
    std::cout << "\n";
    std::cout << "   http://www.tuusniemi.fi/~ef \n";
}

void logo()
{
    int a = 0, oclock;
    load_efp( "EFPS/EFLOGO.EFP", picture, 0 );
    load_efp_pal( "EFPS/EFLOGO.EFP", pal );
    setpal( pal, 0 );
    memcpy( screen, picture, 64000 );
    tk_port::event_tick();
    oclock = clock();
    while (a < 255)
        if ( oclock != clock())
        {
            oclock = clock();
            a += 10;
            setpal( pal, a );
        }
    while (i::get_last_key() == i::NOTHING && abs((int) (oclock - clock())) < 500)
    {
        tk_port::event_tick();
    }
    while (a > 0)
        if ( oclock != clock())
        {
            oclock = clock();
            a -= 10;
            setpal( pal, a );
        }
    tk_port::event_tick();
}

int main( int argc, char *argv[] )
{
    int a = 0, b;
    int pause = 0;
    for ( a = 1; a < argc; a ++  )
    {
        strupr( argv[a] );
        if ( ( argv[a][0] == '-')||( argv[a][0] == '/'))
        switch( argv[a][1] )
        {
            case 'F':b = atoi( argv[ ++ a] );
                     if ( b >= 30 &&b <= 500 ) target_frames = b;
                     break;
            case 'L':b = atoi( argv[ ++ a] );
                     if ( b > 0 ) START_LEVEL = b - 1;
                     break;
            case 'P':pause = 1;
                     break;
            case 'M':SHOW_ENEMIES = 1;
                     break;
            case 'H':case '?':
                     std::cout << std::endl;
                     std::cout << "Ultimate Tapan Kaikki " << Version << std::endl;
                     std::cout << "Compiled at "<< __DATE__<< ' ' << __TIME__ << std::endl;
                     std::cout << "parameters:" << std::endl;
                     std::cout << "-?,-h this help"<< std::endl;
                     std::cout << "-v    force mcga mode" << std::endl;
                     std::cout << "-p    pause at startup" << std::endl;
                     std::cout << "-m    show enemies on map" << std::endl;
                     std::cout << "-l ## start at level ##" << std::endl;
                     std::cout << "-f ## set gamespeed to ## fps (def. 40)" << std::endl;
                     std::cout << std::endl;
                     exit( 1 );
                     break;
        }
    }
    if(tk_port::init() != 0) {
        return 1;
    }
    srand( (unsigned) time( NULL ) );
    do_all();
    i::clear_stack();
    if ( pause ) i::wait_for_keypress();
    logo();
    menu();
    if ( ph ) MIDASstopModule( ph );
    free_all();
    tk_port::deinit();
//  credits();
    return 0;
}
