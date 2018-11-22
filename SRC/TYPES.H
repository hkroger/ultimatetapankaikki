#ifndef __TK__TYPES__
#define __TK__TYPES__

struct special
{
    int num;
    char text[15];
};

struct BLOCK
{
    int type;
    int num;
    int shadow;
};

struct s
{
    const char *name;
    int cost;
    int max;
};

struct WEapon
{
    const char *name;
    int cost;
    int loading_time;
    int smoke;
    int bullet_amount;
    int angle_add;
    int gun;
    int btype;
    int sound;
};

struct juu
{
    char name[15];
    char *spr;
    int speed;
    int curr_weapon;
    int energy;
    int reward;
    int explo_death;
};

struct vile
{
    char name[13];
};

#endif
