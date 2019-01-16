#ifndef __TK__CLASSES__
#define __TK__CLASSES__
#include <SDL.h>
#include "DEFINES.H"
#include "TYPES.H"
#include "NET/NET.H"
#include "INPUT/INPUT.H"

class Crate_info
{
 public:
  int type1;
  int type2;
  int x,y;
};

class Message_board
{
 public:
  int messages;
  int time_count;
  char message[MAX_MESSAGES][MAX_MESSAGE_LENGTH];
  void add_message(char msg[MAX_MESSAGE_LENGTH]);
  void draw();
  void clear();
};

class Crate
{
    public:
    int type;
    int weapon;
    int bullet;
    int energy;
    int x, y;
    int IN_USE;
    int text_angle;
    int text_count;
    int wake_up_count;
    float text_x, text_y;
    char text[20];
    void move_text();
    void chk_wake_up(int);
};

class General_level_info
{
    public:
    char comment[20];
    int time_limit;// seconds
    int enemies[DIFF_ENEMIES];
};

class Normal_game_level_info
{
    public:
    int weapon_crates[DIFF_WEAPONS];
    int bullet_crates[DIFF_BULLETS];
    int energy_crates;
};

class Death_match_level_info
{
    public:
    int weapon_crates[DIFF_WEAPONS];
    int bullet_crates[DIFF_BULLETS];
    int energy_crates;
};

class Spot
{
    public:
    int x;
    int y;
    int size;
};

class Steam
{
    public:
    int x;
    int y;
    int angle;
    int speed;
};

class Light_table
{
    public:
    char data[256][16];
};

class Body_part
{
    public:
    float x, y;
    int angle;
    int num;
    float speed;
    int IN_USE;
    void move();
};

class Effect_type
{
    public:
    int index;
    int tcount;
    int order;
    int delay;
    int random;// boolean
    int xs, ys, offs;
};

class Bullet_type
{
    public:
    char name[20];
    int type;
    int stop;
    int speed;
    float power;
    int time;
    int explos;// power of explosive
    int push_power;
    int cost;
    int amount_in_crate;
    int mul;
    int sleep_time;
    int max;
    char *sprite;
    int xs, ys, offs, xsize;
    Light_table *light;
    int lsize;
    int llum;
    float ladd;
    int effect;
};

class Effect
{
    public:
    float x, y;
    int angle;
    int speed;
    Effect_type *type;
    int count;
    int IN_USE;
    Light_table *lt;
    int age;
    void roll();
};

class Player
{
    public:
    int tindex;
    float x, y;
    int enabled;
    char *see_level;
    int time_since_last_alive;
    int speed, angle, rangle, rangle2, scr_x, scr_y;
    int weapons[DIFF_WEAPONS + 1];
    int curr_weapon, load_cnt;
    int shoot_cnt;
    int bullets[DIFF_BULLETS];
    int cash;
    int max_energy;
    float energy;
    int shield;
    int color;
    int burning;
    Player *Frying_player;
    nodeaddr node;
    char name[10];
    int HIT, WALK, FIRE, DEAD, KICK, ANIM;
    tk_port::input::Input K_LEFT, K_RIGHT, K_UP, K_DOWN, K_SHOOT, K_SHIFT, K_STRAFE, K_LSTRAFE, K_RSTRAFE;
    int TARGET;
    float PUSH_POWER;
    int PUSH_ANGLE;
    int kills[DIFF_ENEMIES];
    int total_kills;
    int shooted, hitten;
    int player_kills;
    void footprint( int side );
    void kick_body_parts();
    void chk_keys( int index );
    void animate();
    void move( int angle, float spd, int index );
    void shoot();
    void load();
    void change_weapon();
    void get_damage( float damage, Player *P );
    void rotate( int change );
    void move_scr();
    void shop();
    void see();
    void random_place();
    void death_match_start_up();
};

class Bullet
{
    public:
    float x, y;
    Bullet_type *type;
    Player *P;
    int index;
    int angle;
    int time;
    int count;
    int IN_USE;
    void move();
};

class Keys
{
    public:
    tk_port::input::Input K_LEFT, K_RIGHT, K_UP, K_DOWN, K_SHOOT, K_SHIFT, K_STRAFE, K_LSTRAFE, K_RSTRAFE;
    void change();
};

class Enemy
{
    public:
    float x, y;
    int angle, rangle, tangle, walk_cnt, load_cnt;
    float energy;
    int shoot_cnt;
    int angle_pl[MAX_PLAYERS];
    int type;
    int burning;
    Player *Frying_player;
    float PUSH_POWER;
    int PUSH_ANGLE;
    int WALK, FIRE, SEE_PL[MAX_PLAYERS], DEAD, ANIM;
    int see_somebody;
    void footprint( int side );
    void animate();
    void move( int angle, int spd );
    void shoot();
    void rotate();
    void get_angles();
    void get_damage( float damage, Player *P );
    void try_see_players();
    void load();
    void chk_for_shoot();
};

class Light
{
    public:
    char *data;
    void make( unsigned int size );
};

class Level
{
    public:
    int version;
    void load( char name[13] );
};

#endif
