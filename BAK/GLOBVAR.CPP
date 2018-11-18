#include "defines.h"
#include "types.h"
#include "classes.h"
#include <time.h>
#include <keyb.hpp>

int BACK_TO_MENU=0;

int VESA2=0,FORCE_0x13=0;
int scr_y_size=200;

int ipxavail=0;
int GAME_MODE=0,NETWORK_MODE=CLIENT;
int KILLING_MODE=0;
int SHOW_ENEMIES=0;
int target_cost=500;  // t„ht„imen hinta
volatile unsigned framecount=0;
int effect_age_count;

int dust_blocks[DUST_BLOCKS]={0,
							  16*4,
							  16*5,
							  16*5+2,
							  16*6+1,
							  16*6+1,
							  16*6+2,
							  16*7+1,
							  16*7+2,
							  16*7+3};

int Light_sizes[LIGHTS]={
   15,20,25,30,35,40,45,50,55,60
   };

int DARK_MODE=1;
int LIGHT_EFFECTS=1;
int SHADOWS=1;
int SMOKE_EFFECTS=1;

int sell_price[DIFF_WEAPONS+2]; // myyntihinnat muuttuvia

struct BLOCK *level;

char *see_level;

char cratespr[320*14],
     shadowspr[320*20],
     c4spr[8*8],
     grenadespr[7*7],
	 targetspr[7*7],
	 floorspr[64000],
	 wallspr[64000],
	 floorspr2[64000],
	 rambospr[1160*667],
	 bodypartspr[320*20],
	 effectspr[320*82],
	 pal[256*3],
     picture[64000],
     warespr[64000],
	 enemyspr0[1160*232],
	 enemyspr1[1160*232],
	 enemyspr2[1160*232],
	 enemyspr3[1160*232],
	 enemyspr4[1160*232],
	 enemyspr5[1160*232],
	 enemyspr6[1160*232],
	 minespr[7*7];

char name1[10],name2[10];

char trans_table[256][256];
char shadow_table[256][16];

char *menu_text[Menu_texts]={"single pl game",
							 "multiplayer game",
							 "options",
							 "exit to dos"};

int first=0,all_dead=0,quit=0,MAP_ON=0,next_level=0,complete_time=0;
char DEATH_MATCH_LEVEL[13];
int DEATH_MATCH_EPISODE=0;
int DEATH_MATCH_SPEED;
int episode=0;

int EFFECT_VOLUME=63;
int MUSIC_VOLUME=63;

clock_t st,et;

int F_PER_SEC,OVER_POWER;

int START_LEVEL;
int LEVELS;

int Spot_amount;
int Steam_amount;

int Spot_count;
int Steam_count;

int level_x_size,level_y_size;

int pl_start_x[MAX_PLAYERS],pl_start_y[MAX_PLAYERS];

int PLAYERS;
int ACTIVE_PLAYERS;
int ENEMIES,LEVEL,DEAD_ENEMIES;
int ENEMIES_ON_GAME=1;

int FRAMES_ON;
int target_frames=TARGET_FRAMES; // pelin p„ivitys nopeus
int delau=10;

char *k_text[DIFF_K]={
 "walk forward",
 "walk backward",
 "turn c.clockw.",
 "turn clockw.",
 "fire",
 "change weapon",
 "strafe",
 "strafe left",
 "strafe right",
 "back to menu"
};

struct special spec[SPEC_K]={
  1,"esc",
  15,"tab",
  28,"enter",
  29,"left ctrl",
  90,"right ctrl",
  42,"left shift",
  54,"right shift",
  57,"space",
  58,"caps lock",
  56,"alt",
  92,"alt gr",
  93,"home",
  94,"arrow up",
  100,"page down",
  95,"page up",
  96,"arrow left",
  97,"arrow right",
  98,"end",
  99,"arrow down",
  101,"insert",
  102,"delete"
 };

Effect_type effect_type[10];

char virbuff[320*240];
float sini[360],cosi[360];
char *thefonts[10];
int f_x_size[10],f_y_size[10];

int FONT_NUM;
KeyboardHandler k;
Keys keys1,keys2;
Player player[MAX_PLAYERS];	   // PLAYERS 
Body_part body_part[MAX_BODY_PARTS];
Enemy enemy[MAX_ENEMIES];
Effect effect[MAX_EFFECTS];
Bullet bullet[MAX_BULLETS];
Spot spot_light[MAX_SPOTS];
Steam steam[MAX_STEAM];
Light light[LIGHTS];
Light_table normal_l,red_l,yellow_l,explo_l;
General_level_info     general_level_info;
Normal_game_level_info normal_game_level_info;
Death_match_level_info death_match_level_info;
Crate crate[MAX_CRATES];

MIDASsample magnumwav,
            shotgunwav,
			pistolwav,
			sinkowav,
			hitwav,
			explowav,
			klikwav,
			deathwav,
			wooshwav,
			flamewav
			;

MIDASmodule biisi;

MIDASmodulePlayHandle ph;

int sample_rates[DIFF_WEAPONS+1]={
 10000,		   // nyrkki
 30000,
 13000,
 22000,
 16000,
 17000,
 18000,
 16000,
 15000,
 0,
 11025,
 0
};

struct WEapon weapon[DIFF_WEAPONS+1]={
				  "fist",
				  0,
				  10,
				  0,
				  1,
				  1,
				  0,
				  0,
				  &hitwav,
	     		  "Pistola",
				  400,
				  10,
				  1, 
				  1,
				  10,
				  1,
				  1,
				  &magnumwav,
				  "Shotgun",
				  1000,
				  17,
				  1,
				  6,
				  20,
				  1,
				  3,
				  &shotgunwav,
				  "Uzi",
				  2000,
				  4,
				  1,
				  1,
				  10,
				  1,
				  1,
				  &pistolwav,
				  "Auto rifle",
				  4000,
				  5,
				  1,
				  1,
				  10,
				  1,
				  2,
				  &pistolwav,
				  "Grenade launcher",
				  4000,
				  30,
				  0,
				  1,
				  10,
				  1,
				  5,
				  &sinkowav,
				  "Auto grenadier",
				  6000,
				  10,
				  0,
				  1,
				  10,
				  1,
				  4,
				  &sinkowav,
				  "Heavy launcher",
				  6000,
				  40,
				  0,
				  1,
				  10,
				  1,
				  6,
				  &sinkowav,
				  "Auto shotgun",
				  6000,
				  5,
				  1,
				  6,
				  20,
				  1,
				  3,
				  &shotgunwav,
				  "C4-Activator",
				  1000,
				  10,
				  0,
				  1,
				  1,
				  0,
				  7,
				  0,
				  "Flame thrower",
				  3000,
				  1,
				  0,
				  1,
				  10,
				  1,
				  8,
				  &flamewav,
				  "Mine dropper",
				  1000,
				  20,
				  0,
				  1,
				  1,
				  0,
				  9,
				  0
				  };

Bullet_type bullet_type[DIFF_BULLETS+2];


struct s shield_data ={
		  "Shield",
		  160,
		  15}; // muuta t„t„ shop.h:ssa

struct juu enemy_info[DIFF_ENEMIES]={
	"pistol boy",	
	enemyspr0,
	2,
	1,
	18,
	200,
	"shotgun maniac",	
	enemyspr1,
	2,
	2,
	28,
	350,
	"uzi rebel",	
	enemyspr2,
	2,
	3,
	40,
	400,
	"commando",	
	enemyspr3,
	3,
	4,
	50,
	600,
	"grenade gay",	
	enemyspr4,
	2,
	5,
	40,
	700,
	"civilian",	
	enemyspr5,
	2,
	0,
	10,
	50,
	"punisher",	
	enemyspr6,
	1,
	8,
	150,
	650
};
Level leveldata;

char *player_opt_text[num_player_options]={
	   "define keys 1",
	   "define keys 2",
	   "change pl1 name",
	   "change pl2 name",
	   "previous menu"
	   };

char *game_opt_text[num_game_options]={
	   "darkness",
	   "light effects",
	   "shadows",
	   "previous menu"
	   };

char *sound_opt_text[num_sound_options]={
	   "music volume",
	   "effects volume",
	   "previous menu"
	   };

char *opt_text[num_options]={
	   "player options",
	   "game options",
	   "sound options",
	   "deathmatch options",
	   "previous menu"
	   };

char *deathmatch_opt_text[num_deathmatch_options]={
	   "moving",
	   "enemies",
	   "previous menu"
	   };

char *multip_opt_text[num_multip_options]={
	   "start game",
	   "join game",
	   "episode",
	   "level",
	   "game type",
	   "game mode",
	   "previous menu"
	   };

vile filelist[256][1024];
vile dirlist[256];
int files_in_episode[256];
int dirs=0;


