#ifndef __TK__DEFINES__
#define __TK__DEFINES__

#define Version "v1.2"  

#define pi 3.14159265358979
#define MAX_PLAYERS 8	// max amount of players in game

#define DIFF_BULLETS 9
#define DIFF_WEAPONS 11
#define DIFF_ENEMIES 7

#define ONE_PLAYER 0
#define SPLIT_SCREEN 1
#define NETWORK 2

#define COOPERATIVE 0
#define DEATHMATCH 1

#define NORMAL 0	// panos tyyppejä
#define EXPLOSIVE 1
#define INCENDIARY 2

#define FLOOR 0		  // blockki tyyppejä
#define WALLS 1

#define MAX_ENEMIES 100	// max amount of enemies in game
#define MAX_BODY_PARTS 300 // max amount of bodyparts in game
#define MAX_EFFECTS 300	// max amount of effects in game
#define MAX_BULLETS 300	// max amount of bullets in game
#define MAX_SPOTS 100	// max amount of spot lights in game
#define MAX_STEAM 100	// max amount of steams in game
#define MAX_CRATES 100	// max amount of crates in game

#define WALK_DELAY 20  // walk animation delay
#define DEAD_DELAY 15  // dead animation delay
#define C_DELAY 300    // level complete delay

#define ROT_SPEED 9	// computer players rotate speed

#define M_BLUE 113		  // punainen
#define M_RED 64	 // sinertävä
#define M_YELLOW 144	   // keltainen

#define TIME_ADD 1.1 // action time'll be multiplied with this
#define LIGHTS 10
#define TARGET_FRAMES 40

#define DUST_BLOCKS 10
#define Menu_texts 4

#define SERVER 1
#define CLIENT 0
#define RECEIVERS 10

#define num_options 5
#define num_player_options 5
#define num_game_options 4
#define num_sound_options 3
#define num_deathmatch_options 3
#define num_multip_options 7

#define DIFF_K 10
#define SPEC_K 21

// Effects...

#define BLOOD 0	  // effects
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


#endif
