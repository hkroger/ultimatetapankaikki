#ifndef __TK__TYPES__
#define __TK__TYPES__

#include <midasdll.h>

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
		 char *name;
		 int cost;
         int max;
};

struct WEapon{
		 char *name;
		 int cost;
		 int loading_time;
         int smoke;
         int bullet_amount;
         int angle_add;
         int gun;
         int btype;
         MIDASsample *sound;
};

struct juu
{
	char name[15];
	char *spr;
	int speed;
	int curr_weapon;
	int energy;
   	int reward;
};

struct vile{
 char name[13];
};



#endif
