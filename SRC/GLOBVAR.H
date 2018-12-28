#ifndef __TK__GLOBVAR__
#define __TK__GLOBVAR__
#include <time.h>
#include "PORT.H"
#include "KEYB.H"
#include "DEFINES.H"
#include "NETDEFS.H"
#include "TYPES.H"
#include "CLASSES.H"
#include "SOUND.H"

extern int saved_game_mode;
extern int saved_killing_mode;
extern int net_avail;
extern int BACK_TO_MENU;
extern int scr_y_size, GAME_MODE, NETWORK_MODE;
extern int KILLING_MODE, SHOW_ENEMIES, target_cost;// tähtäimen hinta
extern volatile unsigned framecount;
extern int effect_age_count;
extern int dust_blocks[];
extern int Light_sizes[LIGHTS];
extern int DARK_MODE, LIGHT_EFFECTS, SHADOWS, SMOKE_EFFECTS;
extern int sell_price[DIFF_WEAPONS + 2];// myyntihinnat muuttuvia
extern struct BLOCK *level;
extern char *see_level;
extern char cratespr[], shadowspr[], c4spr[], grenadespr[], targetspr[], floorspr[];
extern char wallspr[], floorspr2[], rambospr[], bodypartspr[], effectspr[];
extern char pal[], picture[], warespr[], enemyspr0[], enemyspr1[], enemyspr2[];
extern char enemyspr3[], enemyspr4[], enemyspr5[], enemyspr6[], enemyspr7[], minespr[];
extern char name1[10], name2[10];
extern char trans_table[256][256];
extern char shadow_table[256][16];
extern const char *menu_text[Menu_texts];
extern int first, all_dead, quit, MAP_ON, next_level, complete_time;
extern char DEATH_MATCH_LEVEL[13];
extern int DEATH_MATCH_EPISODE;
extern int DEATH_MATCH_SPEED;
extern int episode;
extern int EFFECT_VOLUME, MUSIC_VOLUME;
extern clock_t st, et;
extern int F_PER_SEC, OVER_POWER, START_LEVEL, LEVELS;
extern unsigned int Spot_amount, Steam_amount, Spot_count, Steam_count;
extern int level_x_size, level_y_size;
extern int pl_start_x[MAX_PLAYERS], pl_start_y[MAX_PLAYERS];
extern int ACTIVE_PLAYERS, ENEMIES, LEVEL, DEAD_ENEMIES, ENEMIES_ON_GAME;
extern int FRAMES_ON, target_frames, delau;
extern const char *k_text[DIFF_K];
extern struct special spec[SPEC_K];
extern Effect_type effect_type[10];
extern char *screen;
extern char virbuff[];
extern float sini[], cosi[];
extern char *thefonts[10];
extern int f_x_size[10], f_y_size[10];
extern int FONT_NUM;
extern Keys keys1, keys2;
extern Message_board message_board;
extern Player player[MAX_PLAYERS];// PLAYERS
extern Player *aplayer[2];
extern Body_part body_part[MAX_BODY_PARTS];
extern Enemy enemy[MAX_ENEMIES];
extern Effect effect[MAX_EFFECTS];
extern Bullet bullet[MAX_BULLETS];
extern Spot spot_light[MAX_SPOTS];
extern Steam steam[MAX_STEAM];
extern Light light[LIGHTS];
extern Light_table normal_l, red_l, yellow_l, explo_l;
extern General_level_info general_level_info;
extern Normal_game_level_info normal_game_level_info;
extern Death_match_level_info death_match_level_info;
extern Crate crate[MAX_CRATES];
extern Crate_info normal_crate_info[MAX_CRATE_INFO];
extern Crate_info deathmatch_crate_info[MAX_CRATE_INFO];
extern int normal_crate_amount;
extern int deathmatch_crate_amount;
extern Bullet_type bullet_type[DIFF_BULLETS + 2];
extern struct s shield_data;
extern struct WEapon weapon[DIFF_WEAPONS + 1];
extern struct juu enemy_info[DIFF_ENEMIES];
extern MIDASsample samplep[];
extern MIDASmodule biisi[];
extern MIDASmodulePlayHandle ph;
extern int sample_rates[DIFF_WEAPONS + 1];
extern Level leveldata;
extern char player_opt_text[num_player_options][50];
extern char game_opt_text[num_game_options][50];
extern char sound_opt_text[num_sound_options][50];
extern char opt_text[num_options][50];
extern char deathmatch_opt_text[num_deathmatch_options][50];
extern char multip_opt_text[num_multip_options][50];
extern vile filelist[256][1024];
extern vile dirlist[256];
extern int files_in_episode[256];
extern int dirs;
#endif
