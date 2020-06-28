#ifndef __TK__DEFINES__
#define __TK__DEFINES__

// Using __EMSCRIPTEN__ instead of TK_PORT_EMCC because including "PORT.H" in this file
// can cause a clash with CLOCKS_PER_SEC define...
#if defined (__EMSCRIPTEN__)
#define Remark " (web beta, no audio/net)"
#else
#define Remark
#endif

#define Version "v1.21+sp3 SNAPSHOT" Remark
#define pi 3.14159265358979
#define CRATE_WAKE_UP_COUNT 500 // = 12.5 secs if 40 frames per sec
#define MESSAGE_TIME_ON_SCREEN 200  // = 5 secs if 40 frames per sec
#define MAX_MESSAGES 6
#define MAX_MESSAGE_LENGTH 40
#define MAX_PLAYERS 8  // max amount of players in game
#define DIFF_BULLETS 9
#define DIFF_WEAPONS 11
#define DIFF_ENEMIES 8
#define ONE_PLAYER 0
#define SPLIT_SCREEN 1
#define NETWORK 2
#define COOPERATIVE 0
#define DEATHMATCH 1
#define NORMAL 0  // panos tyyppejä
#define EXPLOSIVE 1
#define INCENDIARY 2
#define FLOOR 0 // blockki tyyppejä
#define WALLS 1
#define OUTOFBOUNDS 0xFF
#define MAX_ENEMIES 100  // max amount of enemies in game
#define MAX_BODY_PARTS 300 // max amount of bodyparts in game
#define MAX_EFFECTS 300  // max amount of effects in game
#define MAX_BULLETS 300  // max amount of bullets in game
#define MAX_SPOTS 100  // max amount of spot lights in game
#define MAX_STEAM 100  // max amount of steams in game
#define MAX_CRATES 200  // max amount of crates in game
#define MAX_CRATE_INFO 100  // max amount of pointed crates in game
#define WALK_DELAY 20 // walk animation delay
#define DEAD_DELAY 15 // dead animation delay
#define C_DELAY 300 // level complete delay
#define ROT_SPEED 9  // computer players rotate speed
#define M_BLUE 113 // punainen
#define M_RED 64 // sinertävä
#define M_YELLOW 144 // keltainen
#define TIME_ADD 1.1 // action time'll be multiplied with this
#define LIGHTS 10
#define TARGET_FRAMES 40 // framerate
#define DUST_BLOCKS 10
#if defined (__EMSCRIPTEN__)
#define Menu_texts 3 // No Quit
#else
#define Menu_texts 4
#endif
#define num_options 4
#define num_player_options 5
#define num_game_options 4
#define num_sound_options 3
#define num_deathmatch_options 4
#define num_multip_options 7
#define DIFF_K 10
#define INVALID_EPISODE -1
 // Effects...
#define BLOOD 0
#define SMOKE 1
#define EXPLO 2
#define METAL 3
#define SMOKE2 4
#define BLOOD2 5
#define SMOKE3 6
#define STEAM 7
#define TELEPORT 8
#define FOOTPRINT 9
#define EXPLO_RAY 10
// Sounds...
#define DIFF_WAVS 10

#define MAGNUMWAV 1
#define SHOTGUNWAV 2
#define PISTOLWAV 3
#define SINKOWAV 4
#define HITWAV 5
#define EXPLOWAV 6
#define KLIKWAV 7
#define DEATHWAV 8
#define WOOSHWAV 9
#define FLAMEWAV 10

#endif
